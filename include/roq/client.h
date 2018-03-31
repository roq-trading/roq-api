/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>
#include <roq/codec.h>
#include <roq/libevent.h>
#include <roq/logging.h>
#include <roq/platform.h>
#include <roq/stream.h>

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <list>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#if defined(HAVE_CROSSGUID)
#include <crossguid/guid.hpp>
#endif

namespace roq {
namespace client {

// Connection
// parse connection string: [user[:password]@][address]
// type:
//   unix  [path]
//   tcp   address=hostname:port
class Connection final {
 public:
  Connection(const std::string& address,
      const std::string& user, const std::string& password)
      : _address(address), _user(user), _password(password) {}
  static Connection create(const std::string& connection_string) {
    size_t offset = 0;
    std::string user, password;
    auto at = connection_string.find('@', offset);
    if (at != std::string::npos) {
      auto colon = connection_string.find(':', offset);
      if (colon != std::string::npos && colon < at) {
        user.assign(connection_string, offset, colon - offset);
        password.assign(connection_string, colon + 1, at - colon - 1);
      } else {
        user.assign(connection_string, offset, at - offset);
        // no password (empty string)
      }
      offset = at + 1;
    }
    if (connection_string[offset] == '/') {  // unix domain
      std::string path(connection_string, offset);
      Connection result(path, user, password);
      return result;
    } else {  // network end-point
      auto colon = connection_string.find(':', offset);
      if (colon != std::string::npos)
        throw std::runtime_error("Expected hostname:port");
      std::string hostname(connection_string, offset, colon - offset);
      auto port = atoi(connection_string.data() + colon + 1);
      throw std::runtime_error("Not implemented");
    }
  }
  const net::Address& get_address() const { return _address; }
  const std::string& get_user() const { return _user; }
  const std::string& get_password() const { return _password; }

 private:
  net::Address _address;
  std::string _user;
  std::string _password;
};

// Gateways
// parse name to connection string: {name=connection}
struct Gateways final {
  typedef std::unordered_map<std::string, Connection> gateways_t;
  static gateways_t create(const std::string& gateways) {
    gateways_t result;
    size_t offset = 0;
    while (true) {
      auto assignment = gateways.find('=', offset);
      if (assignment == std::string::npos)
        throw std::runtime_error("An assignment operator was expected");
      auto separator = gateways.find(',', offset);
      auto last = separator == std::string::npos;
      if (!last && (separator < assignment))
        throw std::runtime_error("Assignment must precede separator");
      std::string key(gateways, offset, assignment - offset);
      auto length = (last ? gateways.length() : separator) - (assignment + 1);
      std::string connection_string(gateways, assignment + 1, length);
      result.emplace(key, Connection::create(connection_string));
      if (last)
        break;
      offset = separator + 1;
    }
    return result;
  }
};

// Controller
template <typename T>
class Controller final {
  typedef std::unordered_map<std::string, Connection> gateways_t;

 public:
  explicit Controller(const gateways_t& gateways) : _gateways(gateways) {}
  explicit Controller(gateways_t&& gateways) : _gateways(std::move(gateways)) {}
  template <typename... Args>
  void create_and_dispatch(Args&&... args) {
    const char *trace_source = "";
    Dispatcher(_gateways, std::forward<Args>(args)...).dispatch();
  }

 private:
  // Statistics
  struct Statistics final {
    uint64_t messages_sent = 0;
    uint64_t messages_received = 0;
    uint64_t connections_succeeded = 0;
    uint64_t connections_failed = 0;
  };  // Statistics

 private:
  // Dispatcher
  class Dispatcher final : public Strategy::Dispatcher {
    // Gateway
    class Gateway final : public codec::EventHandler {
     public:
      Gateway(const std::string& name, const Connection& connection,
              Strategy& strategy, libevent::Base& base,
              codec::Buffer& buffer, codec::Encoder<uint64_t>& encoder,
              Statistics& statistics,
              std::unordered_set<Gateway *>& callbacks,
              std::string& uuid)
          : _name(name), _connection(connection), _strategy(strategy),
            _base(base), _buffer(buffer), _encoder(encoder),
            _statistics(statistics),
            _event_dispatcher(*this),
            _event_decoder(_event_dispatcher),
            _callbacks(callbacks), _state(Disconnected),
            _retries(0), _retry_timer(0),
            _uuid(uuid) {
      }
      bool ready() const {
        return _state == Ready;
      }
      bool refresh() {
        LOG_IF(FATAL, _retry_timer < 0) << "Retry timer should never become negative";
        if (_retry_timer > 0 && 0 != --_retry_timer)
          return false;
        switch (_state) {
          case Disconnected:
            return try_connect();
          case Failed:
            return reset();
          default:
            LOG(FATAL) << "Should never get here!";
            std::abort();  // FIXME(thraneh): avoid compiler warnings until LOG(FATAL) has been fixed
        }
      }
      void send(codec::Queue& queue, bool internal) {
        if (_state < Connected) {
          LOG(FATAL) << "[" << _name << "] Not connected. Unable to send the request";
        } else if (!internal && _state < Ready) {
          LOG(FATAL) << "[" << _name << "] Not ready. Unable to send the request";
        }
        auto is_cached = false;
        _writer.write(_write_buffer, queue, is_cached);
        try {
          _buffer_event->write(_write_buffer);
          _statistics.messages_sent += queue.size();
        } catch (libevent::RuntimeError& e) {
          LOG(WARNING) << "[" << _name << "] Caught exception, what=\"" << e.what() << "\"";
          write_failed();
          ++_statistics.connections_failed;
          LOG(WARNING) << "[" << _name << "] Failed write attempt. Unable to send the request";
          throw std::runtime_error("Unable to send the request");
        }
      }

     private:
      bool try_connect() {
        LOG(INFO) << "[" << _name << "] Connecting to " << _connection.get_address().to_string();
        increment_retries();
        try {
          connect();
          _state = Connecting;
          return true;  // remove
        } catch (libevent::RuntimeError& e) {
          LOG(WARNING) << "[" << _name << "] Caught exception, what=\"" << e.what() << "\"";
          reset_retry_timer();
          return false;
        }
        LOG(FATAL) << "Should never get here!";
      }
      bool reset() {
        _state = Disconnected;
        reset_buffers();
        reset_retry_timer();
        schedule_async_callback();
        return false;
      }
      void connection_succeeded() {
        LOG(INFO) << "[" << _name << "] Connection succeeded";
        _state = Connected;
        reset_retries();
        // notify strategy
        ConnectionStatusEvent event {
          .source = _name.c_str(),
          .connection_status = ConnectionStatus::Connected,
        };
        VLOG(1) << "[" << _name << "] ConnectionStatusEvent " << event;
        _strategy.on(event);
        ++_statistics.connections_succeeded;
        auto application = platform::get_program();
        auto hostname = platform::get_hostname();
        Handshake handshake {
          .api_version = ROQ_VERSION,
          .application = application.c_str(),  // TODO(thraneh): controller argument
          .hostname = hostname.c_str(),
          .pid = static_cast<uint32_t>(platform::get_pid()),
          .uuid = _uuid.c_str(),
          .login = _connection.get_user().c_str(),
          .password = _connection.get_password().c_str(),
          .subscriptions = {},  // TODO(thraneh): how to get from the strategy?
        };
        codec::Queue queue(_buffer);
        _encoder.encode(queue, handshake);
        send(queue, true);
      }
      void connection_failed() {
        if (_state >= Connected) {
          LOG(INFO) << "[" << _name << "] Disconnected";
          // notify strategy
          ConnectionStatusEvent event {
          .source = _name.c_str(),
            .connection_status = ConnectionStatus::Disconnected,
          };
          VLOG(1) << "[" << _name << "] ConnectionStatusEvent " << event;
          _strategy.on(event);
        } else {
          LOG(INFO) << "[" << _name << "] Connection attempt " << _retries << " failed";
        }
        _state = Failed;
        schedule_async_callback();
        ++_statistics.connections_failed;
      }
      void write_failed() {
        LOG(INFO) << "[" << _name << "] Write failed";
        _state = Failed;
        schedule_async_callback();
      }

     private:
      void reset_retries() {
        _retries = 0;
      }
      void increment_retries() {
        ++_retries;
      }
      void reset_retry_timer() {
        const int delay[8] = { 1, 1, 1, 2, 2, 5, 5, 10 };
        _retry_timer = delay[std::min(_retries, static_cast<int>((sizeof(delay) / sizeof(delay[0])) - 1))];
        LOG(INFO) << "[" << _name << "] Retrying in " << _retry_timer << " second(s)";
      }
      void reset_buffers() {
        _buffer_event.release();
        _read_buffer.drain(_read_buffer.length());
        _write_buffer.drain(_write_buffer.length());
        _event_decoder.reset();
      }
      void schedule_async_callback() {
        _callbacks.insert(this);
      }
      void connect() {
        auto domain = _connection.get_address().get_family();
        net::Socket socket(domain, SOCK_STREAM, 0);
        socket.non_blocking(true);
        LOG_IF(FATAL, _buffer_event != nullptr) <<
            "BufferEvent should have been cleared when last connection attempt failed!";
        auto buffer_event = std::unique_ptr<libevent::BufferEvent>(
          new libevent::BufferEvent(_base, std::move(socket), 0));
        buffer_event->setcb([this](){ on_read(); },
                            [this](int what){ on_error(what); });
        buffer_event->enable(EV_READ);
        buffer_event->connect(_connection.get_address());
        _buffer_event = std::move(buffer_event);
      }
      void on_error(int what) {
        if (what & BEV_EVENT_CONNECTED)
          connection_succeeded();
        else
          connection_failed();
      }
      void on_read() {
        _buffer_event->read(_read_buffer);
        auto messages = _event_decoder.dispatch(_read_buffer, _name.c_str());
        if (messages)
          _statistics.messages_received += messages;
      }

     protected:
      void on(const BatchBeginEvent& event) final {
        VLOG(2) << "[" << _name << "] BatchBeginEvent " << event;
        _strategy.on(event);
      }
      void on(const BatchEndEvent& event) final {
        VLOG(2) << "[" << _name << "] BatchEndEvent " << event;
        _strategy.on(event);
      }
      void on(const HandshakeEvent& event) final {
        LOG(FATAL) << "[" << _name << "] HandshakeEvent " << event;
      }
      void on(const HandshakeAckEvent& event) final {
        LOG(INFO) << "[" << _name << "] HandshakeAckEvent " << event;
        const auto& handshake_ack = event.handshake_ack;
        if (handshake_ack.failure) {
          LOG(ERROR) << "[" << _name << "] "
            "User can not be logged in (failure_reason=\"" << handshake_ack.failure_reason << "\"). "
            "Terminating now!";
          std::exit(EXIT_FAILURE);
        }
        LOG(INFO) << "[" << _name << "] Communication channel has been created";
        _state = Ready;
      }
      void on(const HeartbeatEvent& event) final {
        LOG(FATAL) << "[" << _name << "] HeartbeatEvent " << event;
      }
      void on(const HeartbeatAckEvent& event) final {
        VLOG(1) << "[" << _name << "] HeartbeatAckEvent " << event;
        const auto& heartbeat_ack = event.heartbeat_ack;
        std::chrono::steady_clock::time_point start(
            std::chrono::duration_cast<std::chrono::steady_clock::duration>(
                std::chrono::microseconds(heartbeat_ack.opaque)));
        auto latency = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now() - start).count();
        // TODO(thraneh): record statistics
        VLOG(2) << "[" << _name << "] Latency = " << latency << " usecs (round-trip)";
      }
      void on(const ReadyEvent& event) final {
        VLOG(1) << "[" << _name << "] ReadyEvent " << event;
        _strategy.on(event);
      }
      void on(const GatewayStatusEvent& event) final {
        VLOG(1) << "[" << _name << "] GatewayStatusEvent " << event;
        _strategy.on(event);
      }
      void on(const ReferenceDataEvent& event) final {
        VLOG(1) << "[" << _name << "] ReferenceDataEvent " << event;
        _strategy.on(event);
      }
      void on(const MarketStatusEvent& event) final {
        VLOG(1) << "[" << _name << "] MarketStatusEvent " << event;
        _strategy.on(event);
      }
      void on(const MarketByPriceEvent& event) final {
        VLOG(1) << "[" << _name << "] MarketByPriceEvent " << event;
        _strategy.on(event);
      }
      void on(const TradeSummaryEvent& event) final {
        VLOG(1) << "[" << _name << "] TradeSummaryEvent " << event;
        _strategy.on(event);
      }
      void on(const CreateOrderEvent& event) final {
        LOG(FATAL) << "[" << _name << "] CreateOrderEvent " << event;
      }
      void on(const CreateOrderAckEvent& event) final {
        VLOG(1) << "[" << _name << "] CreateOrderAckEvent " << event;
        _strategy.on(event);
      }
      void on(const ModifyOrderEvent& event) final {
        LOG(FATAL) << "[" << _name << "] ModifyOrderEvent " << event;
      }
      void on(const ModifyOrderAckEvent& event) final {
        VLOG(1) << "[" << _name << "] ModifyOrderAckEvent " << event;
        _strategy.on(event);
      }
      void on(const CancelOrderEvent& event) final {
        LOG(FATAL) << "[" << _name << "] CancelOrderEvent " << event;
      }
      void on(const CancelOrderAckEvent& event) final {
        VLOG(1) << "[" << _name << "] CancelOrderAckEvent " << event;
        _strategy.on(event);
      }
      void on(const OrderUpdateEvent& event) final {
        VLOG(1) << "[" << _name << "] OrderUpdateEvent " << event;
        _strategy.on(event);
      }
      void on(const TradeUpdateEvent& event) final {
        VLOG(1) << "[" << _name << "] TradeUpdateEvent " << event;
        _strategy.on(event);
      }
      void on(const PositionUpdateEvent& event) final {
        VLOG(1) << "[" << _name << "] PositionUpdateEvent " << event;
        _strategy.on(event);
      }

     private:
      Gateway() = delete;
      Gateway(Gateway&) = delete;
      Gateway& operator=(Gateway&) = delete;

     private:
      const std::string _name;
      const Connection& _connection;
      Strategy& _strategy;
      libevent::Base& _base;
      codec::Buffer& _buffer;
      codec::Encoder<uint64_t>& _encoder;
      Statistics& _statistics;
      codec::EventDispatcher _event_dispatcher;
      codec::Decoder<decltype(_event_dispatcher)> _event_decoder;
      std::unique_ptr<libevent::BufferEvent> _buffer_event;
      libevent::Buffer _read_buffer;
      libevent::Buffer _write_buffer;
      codec::Writer _writer;
      std::unordered_set<Gateway *>& _callbacks;
      enum {
        Disconnected,
        Connecting,
        Connected,
        Handshaking,
        Ready,
        Failed
      } _state;
      int _retries;
      int _retry_timer;
      std::string _uuid;
    };  // Gateway

   private:
    static std::string create_uuid() {
#if defined(HAVE_CROSSGUID)
      return xg::newGuid();
#else
      return "NO_CROSSGUID_LIB";
#endif
    }

   public:
    template <typename... Args>
    explicit Dispatcher(const gateways_t& gateways, Args&&... args)
        : _strategy(*this, std::forward<Args>(args)...),  // request handler, then whatever the strategy needs
          _timer(_base, EV_PERSIST, [this](){ on_timer(); }),
          _next_refresh(std::chrono::steady_clock::now() + std::chrono::seconds(1)),
          _next_statistics(_next_refresh),
          _uuid(create_uuid()) {
      for (const auto& iter : gateways) {
        _gateways.emplace_back(iter.first,
            iter.second, _strategy, _base, _buffer, _encoder, _statistics, _callbacks, _uuid);
        Gateway& gateway = _gateways.back();
        _gateways_by_name[iter.first] = &gateway;
        _callbacks.insert(&gateway);
      }
    }
    void dispatch() {
      _timer.add(std::chrono::seconds(1));
      _base.loop(EVLOOP_NO_EXIT_ON_EMPTY);
    }

   private:
    template <typename R>
    void send_helper(const R& request, const std::string& gateway) {
      auto iter = _gateways_by_name.find(gateway);
      if (iter != _gateways_by_name.end()) {
        codec::Queue queue(_buffer);
        _encoder.encode(queue, request);
        (*iter).second->send(queue, false);
      }
    }
    void send(const CreateOrder& create_order, const std::string& gateway) override {
      send_helper(create_order, gateway);
    }
    void send(const ModifyOrder& modify_order, const std::string& gateway) override {
      send_helper(modify_order, gateway);
    }
    void send(const CancelOrder& cancel_order, const std::string& gateway) override {
      send_helper(cancel_order, gateway);
    }
    void on_timer() {
      auto now = std::chrono::steady_clock::now();
      if (refresh(now)) {
        remove_zombie_connections();
        send_heartbeat(now);
      }
      if (statistics(now)) {
        write_statistics();
      }
      TimerEvent timer_event {};
      static_cast<Strategy&>(_strategy).on(timer_event);
    }
    bool refresh(const std::chrono::steady_clock::time_point now) {
      if (now < _next_refresh)
        return false;
      while (true) {
        _next_refresh += std::chrono::seconds(1);
        if (now < _next_refresh)
          return true;
      }
    }
    void remove_zombie_connections() {
      std::list<Gateway *> remove;
      for (const auto iter : _callbacks)
        if ((*iter).refresh())
          remove.push_back(iter);
      if (_callbacks.size() == remove.size()) {
        _callbacks.clear();
      } else {
        for (auto iter : remove)
          _callbacks.erase(iter);
      }
    }
    void send_heartbeat(std::chrono::steady_clock::time_point now) {
      auto opaque =
          std::chrono::duration_cast<std::chrono::microseconds>(
              now.time_since_epoch()).count();
      Heartbeat heartbeat {
        .opaque = opaque,
      };
      codec::Queue queue(_buffer);
      _encoder.encode(queue, heartbeat);
      for (auto& iter : _gateways)
        if (iter.ready())
          iter.send(queue, false);
    }
    bool statistics(std::chrono::steady_clock::time_point now) {
      if (now < _next_statistics)
        return false;
      while (true) {
        _next_statistics += std::chrono::minutes(5);
        if (now < _next_statistics)
          return true;
      }
    }
    void write_statistics() {
      std::cout << std::flush;
      LOG(INFO) << "Statistics("
        "messages_sent=" << _statistics.messages_sent << ", "
        "messages_received=" << _statistics.messages_received << ", "
        "connections_succeeded=" << _statistics.connections_succeeded << ", "
        "connections_failed=" << _statistics.connections_failed <<
        ")";
    }

   private:
    Dispatcher() = delete;
    Dispatcher(Dispatcher&) = delete;
    Dispatcher& operator=(Dispatcher&) = delete;

   private:
    T _strategy;
    libevent::Base _base;
    libevent::Timer _timer;
    Statistics _statistics;
    std::list<Gateway> _gateways;
    std::unordered_map<std::string, Gateway *> _gateways_by_name;
    std::unordered_set<Gateway *> _callbacks;
    std::chrono::steady_clock::time_point _next_refresh;
    std::chrono::steady_clock::time_point _next_statistics;
    codec::Buffer _buffer;
    codec::Encoder<uint64_t> _encoder;
    std::string _uuid;
  };  // Dispatcher

 private:
  Controller() = delete;
  Controller(Controller&) = delete;
  Controller& operator=(Controller&) = delete;

 private:
  gateways_t _gateways;
};  // Controller

}  // namespace client
}  // namespace roq
