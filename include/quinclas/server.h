/* Copyright (c) 2017, Hans Erik Thrane */

#pragma once

#include <glog/logging.h>

#include <quinclas/tradingapi.h>
#include <quinclas/libevent.h>
#include <quinclas/codec.h>

#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace quinclas {
namespace server {

// TODO(thraneh): locking (or two different controller implementations -- sync/non-sync)
// TODO(thraneh): handshake and heartbeat
// TODO(thraneh): timeout -> disconnect

// Controller
template <typename T>
class Controller final {
  typedef std::unordered_set<std::string> handlers_t;

 public:
  explicit Controller(const handlers_t& handlers) : _handlers(handlers) {}
  template <typename... Args>
  void create_and_dispatch(Args&&... args) {
    Dispatcher(_handlers, std::forward<Args>(args)...).dispatch();
  }

 private:
  // Client
  class Client final {
   public:
    typedef std::function<void(std::unique_ptr<Client>&&)> Initializer;
    typedef std::function<void(Client *)> Finalizer;
    Client(libevent::BufferEvent&& buffer_event, Finalizer finalizer,
           common::Server& server, common::Gateway& gateway)
        : _buffer_event(std::move(buffer_event)), _finalizer(finalizer),
          _request_dispatcher(server, gateway) {
      _buffer_event.setcb(on_read, nullptr, on_error, this);
      _buffer_event.enable(EV_READ);
    }
    void send(libevent::Buffer& buffer) {
      _buffer_event.write(buffer);
    }

   private:
    void on_error(int reason) {
      assert(0 == (reason & BEV_EVENT_CONNECTED));  // we don't expect the connection events when created by listener
      _finalizer(this);  // garbage collect
    }
    void on_read() {
        _buffer_event.read(_buffer);
        while (true) {
          const auto envelope = _buffer.pullup(common::Envelope::LENGTH);
          if (envelope == nullptr)
            break;
          const auto length_payload = common::Envelope::decode(envelope);
          const auto bytes = common::Envelope::LENGTH + length_payload;
          const auto frame = _buffer.pullup(bytes);
          if (frame == nullptr)
            break;
          const auto payload = frame + common::Envelope::LENGTH;
          _request_dispatcher.dispatch_request(payload, length_payload);
          _buffer.drain(bytes);
        }
    }

   private:
    static void on_error(struct bufferevent *bev, short what, void *arg) {  // NOLINT
      reinterpret_cast<Client *>(arg)->on_error(what);
    }
    static void on_read(struct bufferevent *bev, void *arg) {
      reinterpret_cast<Client *>(arg)->on_read();
    }

   private:
    Client() = delete;
    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

   private:
    libevent::BufferEvent _buffer_event;
    Finalizer _finalizer;
    common::RequestDispatcher _request_dispatcher;
    libevent::Buffer _buffer;
  };

 private:
  // Service
  class Service final
      : public libevent::Listener::Handler {
   public:
    Service(libevent::Base& base, net::Socket&& socket, typename Client::Finalizer finalizer,
            typename Client::Initializer initializer, common::Server& server, common::Gateway& gateway)
        : _listener(*this, base, 0, -1, std::move(socket)),
          _finalizer(finalizer), _initializer(initializer),
          _server(server), _gateway(gateway) {}

   private:
    void on_accept(libevent::BufferEvent&& buffer_event) override {
      LOG(INFO) << "service: got connection";
      auto client = std::unique_ptr<Client>(new Client(std::move(buffer_event), _finalizer, _server, _gateway));
      _initializer(std::move(client));
    }

   private:
    Service() = delete;
    Service(const Service&) = delete;
    Service& operator=(const Service&) = delete;

   private:
    libevent::Listener _listener;
    typename Client::Finalizer _finalizer;
    typename Client::Initializer _initializer;
    common::Server& _server;
    common::Gateway& _gateway;
  };

 private:
  // Dispatcher
  class Dispatcher final
      : public common::Server,
        public common::Gateway::Dispatcher,
        public libevent::TimerEvent::Handler {
   public:
    template <typename... Args>
    explicit Dispatcher(const handlers_t& handlers, Args&&... args)
        : _gateway(*this, std::forward<Args>(args)...),  // dispatcher, then whatever the gateway needs
          _timer(*this, _base) {
      auto initializer = [this](std::unique_ptr<Client>&& client){ add(std::move(client)); };
      auto finalizer = [this](Client *client){ remove(client); };
      for (auto iter : handlers) {
        unlink(iter.c_str());
        net::Address address(iter);
        net::Socket socket(PF_LOCAL, SOCK_STREAM, 0);
        socket.non_blocking(true);
        socket.bind(address);
        _services.emplace_back(std::unique_ptr<Service>(
              new Service(_base, std::move(socket), finalizer, initializer, *this, _gateway)));
      }
    }
    void dispatch() {
      static_cast<common::Gateway&>(_gateway).start();
      _timer.add({.tv_sec = 1});
      _base.loop(EVLOOP_NO_EXIT_ON_EMPTY);
    }

   protected:
    void send(const common::GatewayStatusEvent& event)  override {
      send_helper(event);
    }
    void send(const common::ReferenceDataEvent& event)  override {
      send_helper(event);
    }
    void send(const common::MarketStatusEvent& event)  override {
      send_helper(event);
    }
    void send(const common::MarketByPriceEvent& event)  override {
      send_helper(event);
    }
    void send(const common::SessionStatisticsEvent& event)  override {
      send_helper(event);
    }
    void send(const common::DailyStatisticsEvent& event)  override {
      send_helper(event);
    }
    void send(const common::CreateOrderAckEvent& event)  override {
      send_helper(event);
    }
    void send(const common::ModifyOrderAckEvent& event)  override {
      send_helper(event);
    }
    void send(const common::CancelOrderAckEvent& event)  override {
      send_helper(event);
    }
    void send(const common::OrderUpdateEvent& event)  override {
      send_helper(event);
    }
    void send(const common::TradeUpdateEvent& event)  override {
      send_helper(event);
    }

   private:
      template <typename E>
      void send_helper(const E& event) {  // TODO(thraneh): threading
        if (_clients.empty())
          return;
        _flat_buffer_builder.Clear();
        _flat_buffer_builder.Finish(common::convert(_flat_buffer_builder, event));
        const auto payload = _flat_buffer_builder.GetBufferPointer();
        const auto length_payload = _flat_buffer_builder.GetSize();
        common::Envelope::encode(_envelope, length_payload);
        std::list<Client *> failures;
        for (auto& iter : _clients) {  // TODO(thraneh): threading
          _buffer.add(_envelope, sizeof(_envelope));
          _buffer.add(payload, length_payload);
          try {
            iter.second->send(_buffer);
          } catch (std::runtime_error& e) {  // TODO(thraneh): maybe a more specific exception type?
            LOG(WARNING) << "dispatcher: caught exception, what=\"" << e.what() << "\"";
            LOG(WARNING) << "dispatcher: failed write attempt -- unable to send the event";
            failures.push_back(iter.first);
          }
        }
        if (failures.empty())
          return;
        for (auto iter : failures)
          remove(iter);  // TODO(thraneh): threading
      }

   protected:
    void on(const common::HandshakeRequest& request) override {
      LOG(INFO) << "got handshake request";
      const common::MessageInfo message_info = {
        .gateway = "XXX",
      };
      const common::HandshakeAck handshake_ack = {
        .api_version = "",
      };
      const common::HandshakeAckEvent event = {
        .message_info = message_info,
        .handshake_ack = handshake_ack,
      };
      send_helper(event);
    }
    void on(const common::HeartbeatRequest& request) override {
      LOG(INFO) << "got heartbeat request";
      const common::MessageInfo message_info = {
        .gateway = "XXX",
      };
      const common::HeartbeatAck heartbeat_ack = {
        .opaque = request.heartbeat.opaque,
      };
      const common::HeartbeatAckEvent event = {
        .message_info = message_info,
        .heartbeat_ack = heartbeat_ack,
      };
      send_helper(event);
    }

   private:
    void add(std::unique_ptr<Client>&& client) {
      _clients.emplace(client.get(), std::move(client));
    }
    void remove(Client *client) {
      LOG(INFO) << "controller: removing client";
      auto iter = _clients.find(client);
      assert(iter != _clients.end());
      _zombies.push_back(std::move((*iter).second));
      _clients.erase(iter);
    }

   private:
    void on_timer() override {
      if (_zombies.empty())
        return;
      LOG(INFO) << "controller: removing " << _zombies.size() << " zombied client(s)";
      _zombies.clear();
    }

   private:
    Dispatcher() = delete;
    Dispatcher(const Dispatcher&) = delete;
    Dispatcher& operator=(const Dispatcher&) = delete;

   private:
    T _gateway;
    libevent::Base _base;
    libevent::TimerEvent _timer;
    std::list<std::unique_ptr<Service> > _services;
    std::unordered_map<Client *, std::unique_ptr<Client> > _clients;
    std::list<std::unique_ptr<Client> > _zombies;
    flatbuffers::FlatBufferBuilder _flat_buffer_builder;
    libevent::Buffer _buffer;
    uint8_t _envelope[common::Envelope::LENGTH];
  };

 private:
  Controller() = delete;
  Controller(const Controller&) = delete;
  Controller& operator=(const Controller&) = delete;

 private:
  const handlers_t& _handlers;
};

}  // namespace server
}  // namespace quinclas
