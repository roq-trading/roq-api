[![Build Status](https://travis-ci.org/quinclas/tradingapi.svg?branch=master)](https://travis-ci.org/quinclas/tradingapi)
[![Coverity](https://scan.coverity.com/projects/14891/badge.svg)](https://scan.coverity.com/projects/quinclas-tradingapi)
[![License: BSD](https://img.shields.io/badge/license-BSD-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
[![Join the chat at https://gitter.im/quinclas/Lobby](https://badges.gitter.im/quinclas/Lobby.svg)](https://gitter.im/quinclas/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

# Generic trading interface to broker APIs.

Copyright (c) 2017-2018, Hans Erik Thrane.

## License

[BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause)

## Overview

This repository contains open sourced APIs and tools.

![overview](https://github.com/quinclas/tradingapi/blob/gh-pages/_images/design.png)

## Documentation

Detailed documentation can be found [here](https://quinclas.github.io/tradingapi/index.html).

## Platforms

* Linux
* macOS (partial support)

## Build Tools

| Tool       | Version  | Purpose |
| ---------- | -------- | ------- |
| autoconf   |          | Build   |
| automake   |          | Build   |
| libtool    |          | Build   |
| pkg-config |          | Build   |
| gcc        | >= 4.8.1 | Build   |
| clang      | >= 3.3   | Build   |
| cmake      | >= 2.6.4 | Test    |

## Dependencies

| Library                                                          | Version  | License                                                      | Purpose                         | Gateways | API | Examples |
| ---------------------------------------------------------------- | -------- | ------------------------------------------------------------ | ------------------------------- |:--------:|:---:|:--------:|
| [breakpad](https://github.com/google/breakpad)                   |          | [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause) | Crash dump (debugging)          |     X    |     |          |
| [cctz](https://github.com/google/cctz)                           | >= 2.1   | [Apache-2.0](https://opensource.org/licenses/Apache-2.0)     | Time-zone conversion            |     X    |  X  |     X    |
| [cpptoml](https://github.com/skystrife/cpptoml)                  | >= 0.4   | [MIT](https://opensource.org/licenses/MIT)                   | Config-file parsing             |          |     |          |
| [double-conversion](https://github.com/google/double-conversion) | >= 3.0   | [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause) | Number conversion               |     X    |  X  |          |
| [flatbuffers](https://github.com/google/flatbuffers)             | >= 1.7   | [Apache-2.0](https://opensource.org/licenses/Apache-2.0)     | Communication protocol          |     X    |  X  |     X    |
| [gflags](https://github.com/gflags/gflags)                       | >= 2.2   | [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause) | Command-line options            |     X    |     |     X    |
| [glog](https://github.com/google/glog)                           | >= 0.3.5 | [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause) | Logging                         |          |  X  |     X    |
| [googletest](https://github.com/google/googletest)               | >= 1.8   | [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause) | Testing                         |     X    |  X  |          |
| [gperftools](https://github.com/gperftools/gperftools)           | >= 2.6   | [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause) | Memory management               |     X    |     |          |
| [libconfig](https://github.com/hyperrealm/libconfig)             | >= 1.5   | [LGPL-2.1](https://opensource.org/licenses/lgpl-2.1.php)     | Config-file parsing             |     X    |     |     X    |
| [libevent](https://github.com/libevent/libevent)                 | >= 2.1   | [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause) | Async event processing          |     X    |  X  |     X    |
| [libucl](https://github.com/vstakhov/libucl)                     | >= 0.8   | [BSD-2-Clause](https://opensource.org/licenses/BSD-2-Clause) | Config-file parsing             |          |     |          |
| [spdlog](https://github.com/gabime/spdlog)                       | >= 0.16  | [MIT](https://opensource.org/licenses/MIT)                   | Logging                         |     X    |  X  |          |

## Building

    ./autogen.sh
    ./configure
    make
    make install

## Conda

You are able to access pre-built binaries from our Conda repositories

* <http://quinclas.com/dist/conda/unstable>
* <http://quinclas.com/dist/conda/stable>

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
    conda config --add channels http://quinclas.com/dist/conda/unstable

    # create your environment
    conda create -y -n my_env_name

    # activate your new environment
    source activate my_env_name

    # install the quinclas tradingapi
    conda install -y quinclas-tradingapi

    # write logs to the console (otherwise you'll find the logs in /tmp)
    export GLOG_logtostderr=1

    # you should now be able to run an example, like this
    example-strategy --local-address /var/tmp/gateway.sock

Of course, you will likely be interested in developing your own strategies.

    # miniconda doesn't do this automatically
    export PKG_CONFIG_PATH=$CONDA_PREFIX/lib/pkgconfig

    # then you can find the necessary CFLAGS like this
    pkg-config --cflags quinclas-tradingapi

## Broker API's

The gateways have not been open sourced.
Pre-built binaries are available from our Conda repositories;

| Gateway  | Package Name      | Access             | Market Data | Order Routing | Linux | Windows | macOS |
| -------- | ----------------- | ------------------ | ----------- | ------------- | ----- | ------- | ----- |
| FemasAPI | quinclas-femasapi | Chinese brokers    | Yes         | Yes           | Yes   | No      | No    |
| TWS API  | quinclas-twsapi   | InteractiveBrokers | Yes         | Yes           | Yes   | No      | Yes   |
| CME MDP  | quinclas-cmemdp   | CME Group          | Yes         | No            | No    | No      | No    |

*Gateways will only work with valid license keys. Please contact us for further details*.
