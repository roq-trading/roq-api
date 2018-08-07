[![Build Status](https://travis-ci.org/quinclas/tradingapi.svg?branch=master)](https://travis-ci.org/roq-trading/roq-api)
[![License: BSD](https://img.shields.io/badge/license-BSD-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
[![Join the chat at https://gitter.im/quinclas/Lobby](https://badges.gitter.im/quinclas/Lobby.svg)](https://gitter.im/quinclas/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

# Generic trading interface

Copyright (c) 2017-2018, Hans Erik Thrane


## License

[BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause)

## Contact

You should [contact us](mailto:info@roq-trading.com) if you want to

* Obtain a gateway license key.
* Discuss a specific gateway implementation.
* Discuss bespoke solutions, e.g. consultancy or joint-venture.


## Introduction

This API is a generic trading interface allowing you to

* Implement your own trading strategies without caring (too much) about the low-level details
  of specific trading API's.
* Easily connect your trading strategy to low-latency market gateways.
* Easily simulate your trading strategy, in-process or out-of-process, using historical market
  data and your own order-matching.

### Implementation

    #include <roq/api.h>

    class Strategy : public roq::Strategy {
     public:
      Strategy(roq::Strategy::Dispatcher& dispatcher, ...)
          : dispatcher(dispatcher) {
        // the dispatcher is the request interface, e.g. for order creation
      }

     protected:
      void on(const roq::[...]Event& event) override {
        // [...]Event handlers allowing you to react to market data updates, order updates, etc.
      }

     private:
      roq::Strategy::Dispatcher& dispatcher;
    };

#### Notes

Event handlers will be invoked from a single thread.
There is no need for you to implement your own locking or queueing.

Your event handler should handle *all* exceptions.
The controller is not supposed to know how to handle exceptions raised by your implementation.
Unhandled exceptions may therefore cause the client process to terminate.

Requests will normally be forwarded from the client to the gateway.
The client should normally receive asynchronous acknowledgement and/or updates.

However, several error conditions specifically pertain to requests

* Requests detected by the client controller to be incorrectly formed, should result in a synchronous exception.
* Attempts to route a request to a disconnected (or non-ready) gateway should result in a synchronous exception.
* Gateways may use the asynchronous acknowledge event to signal further error conditions.
* Lost messages (and timeouts) may occur anywhere along the chain formed by strategy (client), gateway, broker and market.
* Timeout may occur if a request is lost in transit between client and gateway (e.g. disconnect).

On the last point:
The client's controller often can not assume anything if the gateway is disconnected after
a request has been sent and before an acknowledgement has been received.
The gateway may or may not send the acknowledgement and/or the updates following a re-connect.

It is the nature of an asynchronous design: Messages can be lost. Managing timeouts is the way to deal with it.

Your trading strategy implementation should therefore manage exceptions, updates (events) as well as timeouts!

* **Never** expect a request to generate an asynchronous acknowledgement!
* **Always** implement internal checks to verify current state and deal with timeout conditions!

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

We *strongly* suggest using Conda for installing the API.

Reasons for choosing Conda

* Works in user-space and does not require elevated access rights.
* Different binary versions can easily co-exist using Conda's environment management.
* Package dependencies are automatically resolved.
* Version compatibility is automatically verified.
* Conda uses a standardized compiler toolchain to achieve ABI compatilibity.
* Our gateways are delivered as Conda packages.

Conda allows you to install the Roq API (including dependencies) without requiring you to work within
any specific environment.
In other words, it will work for any user on any (Linux) host / virtual machine / container.

Please refer to Conda's documentation on [how to get started](https://conda.io/docs/user-guide/getting-started.html).

### Roq Trading

#### Channels

The following Conda channels are available

| Purpose                 | Channel URL                                  |
| ----------------------- | -------------------------------------------- |
| For prod/uat deployment | <http://roq-trading.com/dist/conda/stable>   |
| For development         | <http://roq-trading.com/dist/conda/unstable> |

For either, you can use specific version numbers to better manage your dependencies.

Please refer to Conda's documentation for [managing channels](https://conda.io/docs/user-guide/tasks/manage-channels.html)
and for [managing packages](https://conda.io/docs/user-guide/tasks/manage-pkgs.html).

Our [examples](https://github.com/roq-trading/roq-samples) include specific details on how to set up a Conda environment,
how to install the Roq API, and how to compile your own code.

#### Packages

The following packages are available

| Name          | Content                                                                       | Availability |
| ------------- | ----------------------------------------------------------------------------- | ------------ |
| roq           | The Roq API (this repo)                                                       | Free         |
| roq-samples   | Compiled version of [roq-samples](https://github.com/roq-trading/roq-samples) | Free         |
| roq-simulator | Gateway simulator                                                             | Free         |
| roq-femasapi  | Gateway based on the FemasAPI                                                 | Licensed     |

For licensed software you'll have to [contact us](mailto:info@roq-trading.com) to obtain a license key.


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

| Library                                                          | Version  | License                                                      | Purpose                         |
| ---------------------------------------------------------------- | -------- | ------------------------------------------------------------ | ------------------------------- |
| [benchmark](https://github.com/google/benchmark)                 | >= 1.4   | [Apache-2.0](https://opensource.org/licenses/Apache-2.0)     | Benchmarking                    |
| [cctz](https://github.com/google/cctz)                           | >= 2.1   | [Apache-2.0](https://opensource.org/licenses/Apache-2.0)     | Time-zone conversion            |
| [crossguid](https://github.com/graeme-hill/crossguid)            | >= 0.2   | [MIT](https://opensource.org/licenses/MIT)                   | UUID                            |
| [flatbuffers](https://github.com/google/flatbuffers)             | >= 1.7   | [Apache-2.0](https://opensource.org/licenses/Apache-2.0)     | Communication protocol          |
| [gflags](https://github.com/gflags/gflags)                       | >= 2.2   | [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause) | Command-line options            |
| [googletest](https://github.com/google/googletest)               | >= 1.8   | [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause) | Testing                         |
| [libevent](https://github.com/libevent/libevent)                 | >= 2.1   | [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause) | Async event processing          |
| [libunwind](https://github.com/libunwind/libunwind)              | >= 1.2   | [MIT](https://opensource.org/licenses/MIT)                   | Stack trace                     |
| [rapidjson](https://github.com/Tencent/rapidjson)                | >= 1.1   | [MIT](https://opensource.org/licenses/MIT)                   | JSON parsing                    |
| [spdlog](https://github.com/gabime/spdlog)                       | >= 1.0   | [MIT](https://opensource.org/licenses/MIT)                   | Logging                         |
| [libucl](https://github.com/vstakhov/libucl)                     | >= 0.8   | [BSD-2-Clause](https://opensource.org/licenses/BSD-2-Clause) | Config-file parsing             |


## Design

![overview](https://github.com/quinclas/tradingapi/blob/gh-pages/_images/design.png)
