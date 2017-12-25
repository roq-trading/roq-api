# Programmatic access to broker API's.

Copyright (c) 2017, Hans Erik Thrane.

## License

[BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause)

## Overview

This repository contains open sourced APIs and tools.

![overview](https://github.com/quinclas/tradingapi/blob/gh-pages/_images/design.png)

## Documentation

Detailed documentation can be found [here](https://quinclas.github.io/tradingapi/index.html).

## Platforms

* Linux
* macOS

*Windows is not yet supported*.

## Build Tools

| Tool     | Version  |
| -------- | -------- |
| autoconf |          |
| automake |          |
| libtool  |          |
| gcc      | >= 4.8.1 |
| clang    | >= 3.3   |

*Indirectly requiring CMake*.

## Dependencies

| Library                                                          | Version  | License                                                      | Purpose                         |
| ---------------------------------------------------------------- | -------- | ------------------------------------------------------------ | ------------------------------- |
| [cctz](https://github.com/google/cctz)                           | >= 2.1   | [Apache-2.0](https://opensource.org/licenses/Apache-2.0)     | Time and time-zone conversion   |
| [double-conversion](https://github.com/google/double-conversion) | >= 3.0   | [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause) | Convert numbers to/from string  |
| [flatbuffers](https://github.com/google/flatbuffers)             | >= 1.7   | [Apache-2.0](https://opensource.org/licenses/Apache-2.0)     | Messaging                       |
| [gflags](https://github.com/gflags/gflags)                       | >= 2.2   | [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause) | Command-line options            |
| [glog](https://github.com/google/glog)                           | >= 0.3.5 | [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause) | Logging                         |
| [googletest](https://github.com/google/googletest)               | >= 1.8   | [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause) | Testing                         |
| [libevent](https://github.com/libevent/libevent)                 | >= 2.1   | [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause) | Async event processing          |

## Building

    ./autogen.sh
    ./configure
    make
    make install

## Conda

You are able to access pre-built binaries from our Conda repositories

* <http://quinclas.com/dist/conda/stable>
* <http://quinclas.com/dist/conda/unstable>

*Conda makes it easy to install programs and libraries without requiring root access.
For further details about Conda, please refer to the official documentation ([link](https://conda.io/docs/))*.

## Example

Here is an example of how to set up your own Conda environment, install a package, and execute a binary.

    # download the installer
    wget -c http://repo.continuum.io/miniconda/Miniconda2-latest-Linux-x86_64.sh

    # install miniconda to your home directory
    bash Miniconda2-latest-Linux-x86_64.sh -b -p ~/miniconda2

    # activate the root environment
    source ~/miniconda2/bin/activate

    # add extra channels for package sources
    conda config --add channels conda-forge
    conda config --add channels http://quinclas.com/dist/conda/stable

    # create your environment
    conda create -y -n my_env_name

    # activate your new environment
    source activate my_env_name

    # install quinclas tradingapi
    conda install -y quinclas-tradingapi

    # write logs to the console
    export GLOG_logtostderr=1

    # you should now be able to run the trading engine
    example-strategy --local-address /var/tmp/gateway.unix

## Broker API's

The gateways have not been open sourced.
Pre-built binaries are available from our Conda repositories;

| Gateway  | Package Name      | Access             | Market Data | Order Routing | Linux | Windows | macOS |
| -------- | ----------------- | ------------------ | ----------- | ------------- | ----- | ------- | ----- |
| FemasAPI | quinclas-femasapi | Chinese brokers    | Yes         | Yes           | Yes   | No      | No    |
| TWS API  | quinclas-twsapi   | InteractiveBrokers | Yes         | Yes           | Yes   | No      | Yes   |
| CME MDP3 | quinclas-cmemdp3  | CME Group          | Yes         | No            | No    | No      | No    |

*Gateways will only work with valid license keys. Please contact us for further details*.
