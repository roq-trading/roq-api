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
#include <unordered_map>
#include <unordered_set>
#include <utility>

namespace quinclas {
namespace simulation {

// Generator
class Generator {
 public:
  Generator() {}
  virtual ~Generator() {}
  virtual std::pair<bool, std::chrono::system_clock::time_point> fetch() = 0;
  virtual void dispatch(common::Strategy& strategy) = 0;

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
      : public common::Strategy::Dispatcher {
   public:
    template <typename... Args>
    explicit Dispatcher(generators_t& generators, Args&&... args)
        : _generators(generators),
          _strategy(*this, std::forward<Args>(args)...) {}
    void dispatch() {
      // TODO(thraneh): support multiple (interleaved) generators
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
    void send(const char *gateway, const common::CreateOrder& create_order) override {
    }
    void send(const char *gateway, const common::ModifyOrder& modify_order) override {
    }
    void send(const char *gateway, const common::CancelOrder& cancel_order) override {
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
}  // namespace quinclas
