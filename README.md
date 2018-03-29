[![Build Status](https://travis-ci.org/quinclas/tradingapi.svg?branch=master)](https://travis-ci.org/roq-trading/roq)
[![License: BSD](https://img.shields.io/badge/license-BSD-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
[![Join the chat at https://gitter.im/quinclas/Lobby](https://badges.gitter.im/quinclas/Lobby.svg)](https://gitter.im/quinclas/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

# Generic trading interface

Copyright (c) 2017-2018, Hans Erik Thrane


## License

[BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause)


## Introduction

This API is a generic trading interface abstraction allowing you to

* Implement your own trading strategy without caring about the details of specific trading API's.
* Connect to low-latency gateways bridging between your trading strategy and specific trading API's.
* Simulate (in-process or out-of-process) historical market data using your own order-matching withoug depending on specific trading API's.

### Implementation

    #include <roq/api.h>

    class Strategy : public roq::common::Strategy {
     public:
      Strategy(roq::common::Strategy::Dispatcher& dispatcher, ...)
          : dispatcher(dispatcher) {
        // the dispatcher is the request interface, e.g. for order creation
      }

     protected:
      void on(const roq::common::[...]Event& event) override {
        // [...]Event handlers allows you to react on market data, order updates, etc.
      }

     private:
      roq::common::Strategy::Dispatcher& dispatcher;
    };

#### Notes

Event handlers will be invoked from a single thread.
There is no need for you to implement your own locking or queueing.

Your event handler should handle *all* exceptions.
The controller is not supposed to know how to handle exceptions raised by your implementation.
Unhandled exceptions may therefore cause the client process to terminate.

Requests will normally be forwarded from the client to the gateway.
The client should normally receive asynchronous acknowledgement and/or updates.

However, several error conditions specifically pertains to requests

* Incorrect requests detected by the client controller should cause synchronous exceptions.
* Disconnected (or non-ready) gateway should cause synchronous exceptions.
* Gateways may use the asynchronous acknowledge event to signal further error conditions.
* Timeouts may occur anywhere between gateway, broker and market.
* Timeout may occur if a request is lost in transit between client and gateway (e.g. disconnect).

On the last point: *The client's controller often can not assume anything if the gateway is disconnected after
a request has been sent and before an acknowledgement has been received*.

Your trading strategy implementation should therefore manage exceptions, events and timeouts!

* **Never** expect a request to actually generate an acknowledgement!
* **Always** implement internal checks (to verify current state) and deal with the timeout conditions!

**It is your responsibility to implement safe order management**.

Please refer to our [examples](https://github.com/roq-trading/roq-samples) for basic checks and design patterns.


### Live Trading

    #include <roq/client.h>
  
    // use e.g. roq::client::Gateways to create this map (details left out)
    std::unordered_map<std::string, Connection> gateways;

    // instantiate the live controller, create the strategy and dispatch
    // note! the constructor allows you to pass further options to the strategy (see roq-samples)
    roq::client::Controller<Strategy>(
        std::move(gateways)).create_and_dispatch();

### Simulation

    #include <roq/simulation.h>

    // a list of bespoke market data simulation generators (details left out)
    std::list<std::unique_ptr<roq::simulation::Generator> > generators;

    // instantiate the simulator, create the strategy and dispatch
    // note! the constructor allows you to pass further options to the strategy (see roq-samples)
    roq::simulation::Controller<Strategy>(
        std::move(generators)).create_and_dispatch();


## Conda

We *strongly* recommend using Conda for installing the API.

Reasons for choosing Conda

* Works in user-space and does not require elevated (root) access.
* Different versions can easily co-exist on the same host.
* Library dependencies are automatically installed.
* Versioning compatibilities are automatically managed.
* Conda standardizes the compiler toolchain to achieve ABI compatilibity.
* Gateway binaries are delivered as Conda packages.

Conda allows you to install the API (including the dependencies) without imposing any specific environment.
In other words, it should work for any user on any Linux host, virtual machine or container.

Roq Trading makes the following Conda repositories available

* <http://roq-trading.com/dist/conda/unstable>
* <http://roq-trading.com/dist/conda/stable>

Please refer to Conda's [documentation](https://conda.io/docs/user-guide/tasks/manage-channels.html) for managing channels.

Our [examples](https://github.com/roq-trading/roq-samples) include specific details on how to get started with Conda.


## Building

If you build from source, you must ensure all the dependencies (listed below) can be found either on the system or from `$PREFIX`.

Finding and installing all the dependencies is no small feat.
You may consider returning to the previous section describing the **Conda** package manager.
You have been **warned**!

Assuming you have all dependencies installed, it's pretty standard

    ./autogen.sh
    ./configure [--prefix $PREFIX]
    make
    make check
    make install


## Compatibility

### Platforms

* Linux
* macOS (partial support)

### Build Tools

| Tool       | Version  | Purpose |
| ---------- | -------- | ------- |
| autoconf   |          | Build   |
| automake   |          | Build   |
| libtool    |          | Build   |
| pkg-config |          | Build   |
| gcc        | >= 4.8.1 | Build   |
| clang      | >= 3.3   | Build   |
| cmake      | >= 2.6.4 | Test    |

### Library Dependencies

| Library                                                          | Version  | License                                                      | Purpose                         | Gateways | API | Examples |
| ---------------------------------------------------------------- | -------- | ------------------------------------------------------------ | ------------------------------- |:--------:|:---:|:--------:|
| [cctz](https://github.com/google/cctz)                           | >= 2.1   | [Apache-2.0](https://opensource.org/licenses/Apache-2.0)     | Time-zone conversion            |     X    |  X  |     X    |
| [crossguid](https://github.com/graeme-hill/crossguid)            | >= 0.2   | [MIT](https://opensource.org/licenses/MIT)                   | UUID                            |     X    |  X  |     X    |
| [double-conversion](https://github.com/google/double-conversion) | >= 3.0   | [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause) | Number conversion               |     X    |  X  |     X    |
| [flatbuffers](https://github.com/google/flatbuffers)             | >= 1.7   | [Apache-2.0](https://opensource.org/licenses/Apache-2.0)     | Communication protocol          |     X    |  X  |     X    |
| [gflags](https://github.com/gflags/gflags)                       | >= 2.2   | [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause) | Command-line options            |     X    |     |     X    |
| [glog](https://github.com/google/glog)                           | >= 0.3.5 | [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause) | Logging                         |          |  X  |     X    |
| [googletest](https://github.com/google/googletest)               | >= 1.8   | [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause) | Testing                         |     X    |  X  |          |
| [gperftools](https://github.com/gperftools/gperftools)           | >= 2.6   | [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause) | Memory management               |     X    |     |          |
| [libevent](https://github.com/libevent/libevent)                 | >= 2.1   | [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause) | Async event processing          |     X    |  X  |     X    |
| [libunwind](https://github.com/libunwind/libunwind)              | >= 1.2   | [MIT](https://opensource.org/licenses/MIT)                   | Stack trace                     |     X    |     |     X    |
| [libwebsockets](https://github.com/warmcat/libwebsockets)        | >= 2.4   | [LGPL-2.1](https://opensource.org/licenses/lgpl-2.1)         | Websockets                      |     X    |     |          |
| [rapidjson](https://github.com/Tencent/rapidjson)                | >= 1.1   | [MIT](https://opensource.org/licenses/MIT)                   | JSON parsing                    |     X    |     |          |
| [spdlog](https://github.com/gabime/spdlog)                       | >= 0.16  | [MIT](https://opensource.org/licenses/MIT)                   | Logging                         |     X    |  X  |          |
| [libucl](https://github.com/vstakhov/libucl)                     | >= 0.8   | [BSD-2-Clause](https://opensource.org/licenses/BSD-2-Clause) | Config-file parsing             |     X    |     |     X    |


## Design

![overview](https://github.com/quinclas/tradingapi/blob/gh-pages/_images/design.png)
