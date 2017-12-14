/* Copyright (c) 2017, Hans Erik Thrane */

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <quinclas/client/event_dispatcher.h>
#include <quinclas/client/request_dispatcher.h>

#include <string>
#include <unordered_map>
#include <vector>

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
  class Dispatcher final : public quinclas::common::Strategy::Dispatcher,
                           public quinclas::io::libevent::TimerEvent::Handler {
   public:
    template <typename... Args>
    explicit Dispatcher(const gateways_t& gateways, Args&&... args)
        : _strategy(*this, std::forward<Args>(args)...),  // first the request handler, then whatever is useful
          _timer(*this, _base) {
      for (const auto iter : gateways) {
        auto res = _gateways.emplace(iter.first, iter.second);  // TODO(thraneh): what about duplicated address?
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
    void on_timer() override {}

   private:
    struct Gateway final {
      explicit Gateway(const std::string& address) : _address(address) {}
      quinclas::io::net::Address _address;
      std::unique_ptr<quinclas::io::libevent::BufferEvent> _bufferevent;
    };

   private:
    T _strategy;
    std::unordered_map<std::string, Gateway> _gateways;
    std::vector<std::string> _disconnected;
    std::vector<std::string> _connecting;
    quinclas::io::libevent::Base _base;
    quinclas::io::libevent::TimerEvent _timer;
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
  // we're ready
  LOG(INFO) << "===== START =====";

  Controller<examples::your::Strategy>({
      { "FEMAS", FLAGS_local_address },
    }).create_and_dispatch(10);

  /*
  // create host-internal socket address
  quinclas::io::net::Address address(FLAGS_local_address.c_str());
  // create host-internal socket
  quinclas::io::net::Socket socket(PF_LOCAL, SOCK_STREAM, 0);
  socket.non_blocking(true);
  // create event base
  quinclas::io::libevent::Base base;
  // TODO(thraneh): create buffer-event inside the event-loop (and recreate upon lost connection)
  // --> need an request dispatcher abstraction to look up the bufferevent given a name
  // create buffer-event
  quinclas::io::libevent::BufferEvent buffer_event(base, std::move(socket));
  // create request dispatcher
  quinclas::client::RequestDispatcher request_dispatcher(buffer_event);
  // create your strategy
  examples::your::Strategy strategy(request_dispatcher);
  // create event dispatcher
  quinclas::client::EventDispatcher event_dispatcher(strategy, base, buffer_event);

  // create timer-event
  quinclas::io::libevent::TimerEvent timer(event_dispatcher, base);
  struct timeval timeout{ .tv_sec = 1, .tv_usec = 0 };
  timer.add(&timeout);
  // connect buffer-event
  buffer_event.connect(address);
  // run event loop
  base.loop(EVLOOP_NO_EXIT_ON_EMPTY);
  */

  // we're done
  LOG(INFO) << "===== STOP =====";
  return EXIT_SUCCESS;  // TODO(thraneh): event-loop should be able to stop with an error code
}
