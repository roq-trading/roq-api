/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

#include <chrono>
#include <list>
#include <stdexcept>
#include <string>
#include <utility>

namespace roq {
namespace simulation {

// Generator
class Generator {
 public:
  Generator() {}
  virtual ~Generator() {}
  virtual std::pair<bool, std::chrono::system_clock::time_point> fetch() = 0;
  virtual void dispatch(Strategy& strategy) = 0;

 private:
  Generator(Generator&) = delete;
  Generator& operator=(Generator&) = delete;
};

// Controller
template <typename T>
class Controller final {
  typedef std::list<std::unique_ptr<Generator> > generators_t;

 public:
  explicit Controller(generators_t&& generators) : _generators(std::move(generators)) {}
  template <typename... Args>
  void create_and_dispatch(Args&&... args) {
    Dispatcher(_generators, std::forward<Args>(args)...).dispatch();
  }

 private:
  // Dispatcher
  class Dispatcher final
      : public Strategy::Dispatcher {
   public:
    template <typename... Args>
    explicit Dispatcher(generators_t& generators, Args&&... args)
        : _generators(generators),
          _strategy(*this, std::forward<Args>(args)...) {}
    void dispatch() {
      // TODO(thraneh): interleave multiple generators
      for (auto& iter : _generators) {
        while (true) {
          auto res = iter->fetch();
          if (res.first) {
            iter->dispatch(_strategy);
          } else {
            return;
          }
        }
      }
    }

   private:
    void send(const CreateOrder& create_order, const std::string& gateway) override {
      throw std::runtime_error("This simulator doesn't support order management");
    }
    void send(const ModifyOrder& modify_order, const std::string& gateway) override {
      throw std::runtime_error("This simulator doesn't support order management");
    }
    void send(const CancelOrder& cancel_order, const std::string& gateway) override {
      throw std::runtime_error("This simulator doesn't support order management");
    }

   private:
    Dispatcher() = delete;
    Dispatcher(const Dispatcher&) = delete;
    Dispatcher& operator=(const Dispatcher&) = delete;

   private:
    T _strategy;
    generators_t& _generators;
  };  // Dispatcher

 private:
  Controller() = delete;
  Controller(const Controller&) = delete;
  Controller& operator=(const Controller&) = delete;

 private:
  generators_t _generators;
};  // Controller

}  // namespace simulation
}  // namespace roq
