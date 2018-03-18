/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <quinclas/codec.h>
#include <quinclas/libevent.h>
#include <quinclas/logging.h>
#include <quinclas/tradingapi.h>

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

#include <crossguid/guid.hpp>

namespace quinclas {
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

const size_t QUEUE_LENGTH = 65536;

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
  class Dispatcher final : public common::Strategy::Dispatcher {
    // Gateway
    class Gateway final : public common::Client {
     public:
      Gateway(const std::string& name, const Connection& connection,
              common::Strategy& strategy, libevent::Base& base,
              common::Queue& queue, common::Encoder& encoder,
              Statistics& statistics,
              std::unordered_set<Gateway *>& callbacks,
              std::string& uuid)
          : _name(name), _connection(connection), _strategy(strategy),
            _base(base), _queue(queue), _encoder(encoder),
            _statistics(statistics),
            _event_dispatcher(*this, strategy),  // HANS -- change !!!
            _event_decoder(_event_dispatcher),
            _callbacks(callbacks), _state(Disconnected),
            _retries(0), _retry_timer(0),
            _uuid(uuid) {}
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
        }
      }
      void send(common::message_t message, bool internal) {
        if (_state < Connected) {
          LOG(FATAL) << "[" << _name << "] Not connected. Unable to send the request";
        } else if (!internal && _state < Ready) {
          LOG(FATAL) << "[" << _name << "] Not ready. Unable to send the request";
        }
        std::vector<common::message_t> messages { message };
        auto is_cached = false;
        _writer.write(_buffer, messages, is_cached);
        try {
          _buffer_event->write(_buffer);
          ++_statistics.messages_sent;
        } catch (std::exception& e) {  // TODO(thraneh): use libevent::RuntimeError
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
        } catch (std::exception& e) {  // TODO(thraneh): use libevent::RuntimeError
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
        common::ConnectionStatusEvent event {
          .source = _name.c_str(),
          .connection_status = common::ConnectionStatus::Connected,
        };
        VLOG(1) << "[" << _name << "] ConnectionStatusEvent " << event;
        _strategy.on(event);
        ++_statistics.connections_succeeded;
        common::Handshake handshake {
          .api_version = QUINCLAS_VERSION,
          .uuid = _uuid.c_str(),
          .login = _connection.get_user().c_str(),
          .password = _connection.get_password().c_str(),
          .subscriptions = {},  // TODO(thraneh): how to get from the strategy?
        };
        auto message = _encoder.encode(handshake);
        send(message, true);
        _queue.reset();
      }
      void connection_failed() {
        if (_state >= Connected) {
          LOG(INFO) << "[" << _name << "] Disconnected";
          // notify strategy
          common::ConnectionStatusEvent event {
          .source = _name.c_str(),
            .connection_status = common::ConnectionStatus::Disconnected,
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
        _buffer.drain(_buffer.length());
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
        _buffer_event->read(_buffer);
        auto messages = _event_decoder.dispatch(_buffer, _name.c_str());  // HANS
        if (messages)
          _statistics.messages_received += messages;
      }

     protected:
      void on(const common::HandshakeAckEvent& event) final {
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
      void on(const common::HeartbeatAckEvent& event) final {
        auto now = std::chrono::system_clock::now();
        auto latency = std::chrono::duration_cast<std::chrono::microseconds>(
            now - event.heartbeat_ack.opaque).count();
        // TODO(thraneh): record statistics
        VLOG(1) << "[" << _name << "] Latency = " << latency << " usecs (round-trip)";
      }

     private:
      Gateway() = delete;
      Gateway(Gateway&) = delete;
      Gateway& operator=(Gateway&) = delete;

     private:
      const std::string _name;
      const Connection& _connection;
      common::Strategy& _strategy;
      libevent::Base& _base;
      common::Queue& _queue;
      common::Encoder& _encoder;
      Statistics& _statistics;
      common::ClientEventDispatcher _event_dispatcher;  // HANS
      common::Decoder<decltype(_event_dispatcher)> _event_decoder;  // HANS
      std::unique_ptr<libevent::BufferEvent> _buffer_event;
      libevent::Buffer _buffer;
      common::Writer _writer;
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

   public:
    template <typename... Args>
    explicit Dispatcher(const gateways_t& gateways, Args&&... args)
        : _strategy(*this, std::forward<Args>(args)...),  // request handler, then whatever the strategy needs
          _timer(_base, EV_PERSIST, [this](){ on_timer(); }),
          _next_refresh(std::chrono::system_clock::now() + std::chrono::seconds(1)),
          _next_statistics(_next_refresh),
          _queue(QUEUE_LENGTH), _encoder(_queue),
          _uuid(xg::newGuid()) {
      for (const auto& iter : gateways) {
        _gateways.emplace_back(iter.first,
            iter.second, _strategy, _base, _queue, _encoder, _statistics, _callbacks, _uuid);
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
    void send(const common::CreateOrder& create_order, const std::string& gateway) override {
      auto iter = _gateways_by_name.find(gateway);
      if (iter != _gateways_by_name.end()) {
        auto message = _encoder.encode(create_order);
        (*iter).second->send(message, false);
        _queue.reset();
      }
    }
    void send(const common::ModifyOrder& modify_order, const std::string& gateway) override {
      auto iter = _gateways_by_name.find(gateway);
      if (iter != _gateways_by_name.end()) {
        auto message = _encoder.encode(modify_order);
        (*iter).second->send(message, false);
        _queue.reset();
      }
    }
    void send(const common::CancelOrder& cancel_order, const std::string& gateway) override {
      auto iter = _gateways_by_name.find(gateway);
      if (iter != _gateways_by_name.end()) {
        auto message = _encoder.encode(cancel_order);
        (*iter).second->send(message, false);
        _queue.reset();
      }
    }
    void on_timer() {
      auto now = std::chrono::system_clock::now();
      if (refresh(now)) {
        remove_zombie_connections();
        send_heartbeat(now);
      }
      if (statistics(now)) {
        write_statistics();
      }
      common::TimerEvent timer_event {
        .utc_time = std::chrono::system_clock::now(),
      };
      static_cast<common::Strategy&>(_strategy).on(timer_event);
    }
    bool refresh(const std::chrono::system_clock::time_point now) {
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
    void send_heartbeat(std::chrono::system_clock::time_point now) {
      common::Heartbeat heartbeat {
        .opaque = std::chrono::time_point_cast<common::duration_t>(now),
      };
      auto message = _encoder.encode(heartbeat);
      for (auto& iter : _gateways)
        if (iter.ready())
          iter.send(message, false);
      _queue.reset();
    }
    bool statistics(std::chrono::system_clock::time_point now) {
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
    std::chrono::system_clock::time_point _next_refresh;
    std::chrono::system_clock::time_point _next_statistics;
    common::Queue _queue;
    common::Encoder _encoder;
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
}  // namespace quinclas
