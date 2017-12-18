/* Copyright (c) 2017, Hans Erik Thrane */

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <quinclas/io/libevent.h>

#include <list>
#include <string>
#include <unordered_map>

#include "your/strategy.h"

DEFINE_string(local_address, "", "host-internal socket address (path)");

// FIXME(thraneh): WIP -- something closer to a real strategy dispatch interface.
namespace {
template <typename T>
class Controller final {
  typedef std::unordered_map<std::string, std::string> gateways_t;

 public:
  explicit Controller(const gateways_t&& gateways) : _gateways(std::move(gateways)) {}
  template <typename... Args>
  void create_and_dispatch(Args&&... args) {
    Dispatcher(_gateways, std::forward<Args>(args)...).dispatch();
  }

 private:
  class Dispatcher final
      : public quinclas::common::Strategy::Dispatcher,
        public quinclas::io::libevent::TimerEvent::Handler {
   public:
    template <typename... Args>
    explicit Dispatcher(const gateways_t& gateways, Args&&... args)
        : _strategy(*this, std::forward<Args>(args)...),  // first, the request handler, then whatever you need
          _timer(*this, _base) {
      // TODO(thraneh): check duplicate address
      for (const auto iter : gateways) {
        auto res = _gateways.emplace(iter.first, Gateway(PF_LOCAL, iter.second));
        if (res.second)
          _disconnected.push_back((*res.first).first);
      }
    }
    void dispatch() {
      _timer.add({.tv_sec = 1, .tv_usec = 0 });
      _base.loop(EVLOOP_NO_EXIT_ON_EMPTY);
    }

   private:
    void send(const quinclas::common::CreateOrderRequest& create_order_request) override {}
    void send(const quinclas::common::ModifyOrderRequest& modify_order_request) override {}
    void send(const quinclas::common::CancelOrderRequest& cancel_order_request) override {}
    void on_timer() override {
      LOG(INFO) << "timer";
      for (const auto iter : _gateways) {
        auto socket = iter.second.create_socket();
        _connecting.push_back(std::unique_ptr<Connection>(
              new Connection(_base, std::move(socket), iter.second._address)));
      }
    }

   private:
    enum State {
      Disconnected,
      Connecting,
      Connected,
      Zombie
    };
    struct Gateway final {
      Gateway(const int domain, const std::string& address) : _domain(domain), _address(address) {}
      quinclas::io::net::Socket create_socket() const {
        quinclas::io::net::Socket socket(_domain, SOCK_STREAM, 0);
        socket.non_blocking(true);
        return socket;
      }
      int _domain;
      quinclas::io::net::Address _address;
    };
    struct Connection final {
      explicit Connection(quinclas::io::libevent::Base& base, quinclas::io::net::Socket socket,
                          const quinclas::io::net::Address& address)
          : _buffer_event(base, std::move(socket)) {
        _buffer_event.setcb(on_read, nullptr, on_error, this);
        _buffer_event.enable(EV_READ);
        _buffer_event.connect(address);
        // FIXME(thraneh): move to member
        // change state from disconnected to connecting
      }

     private:
      static void on_read(struct bufferevent *bev, void *arg) {
        // decode and notify strategy -- should we take strategy as arg to constructor?
      }
      static void on_error(struct bufferevent *bev, short what, void *arg) {  // NOLINT short
        // TODO(thraneh): move self to zombie state
      }

     private:
      quinclas::io::libevent::BufferEvent _buffer_event;
    };

   private:
    T _strategy;
    std::unordered_map<std::string, Gateway> _gateways;
    quinclas::io::libevent::Base _base;
    quinclas::io::libevent::TimerEvent _timer;
    // state management
    std::list<std::string> _disconnected;
    std::list<std::unique_ptr<Connection> > _connecting;
  };

 private:
  gateways_t _gateways;
};
}  // namespace

int main(int argc, char *argv[]) {
  // initialize logging library
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  // parse command-line options
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gflags::ShutDownCommandLineFlags();
  // validate command-line options
  if (FLAGS_local_address.empty())
    throw std::invalid_argument("local_address is missing");
  // ready
  LOG(INFO) << "===== START =====";
  // create framework, instantiate strategy and start event dispatching
  Controller<examples::your::Strategy>({
      { "FEMAS", FLAGS_local_address },
    }).create_and_dispatch(10);
  // done
  LOG(INFO) << "===== STOP =====";
  return EXIT_SUCCESS;  // TODO(thraneh): event-loop should be able to stop with an error code
}
