# Programmatic access to broker API's.

Copyright (c) 2017, Hans Erik Thrane.

## Overview

This repository contains the public interfaces.

![overview](https://github.com/quinclas/tradingapi/blob/gh-pages/_images/design.png)

Detailed documentation available [here](https://quinclas.github.io/tradingapi/index.html).

## Usage

### Platforms

* Linux
* macOS

### Build Tools

| tool     | version  |
| -------- | -------- |
| autoconf |          |
| automake |          |
| libtool  |          |
| gcc      | >= 4.8.1 |
| clang    | >= 3.3   |

*Indirectly requiring CMake*.

### Dependencies

| tool              | version  | license      | source                                                |
| ----------------- | -------- | ------------ | ----------------------------------------------------- |
| cctz              | >= 2.1   | Apache-2.0   | [github](https://github.com/google/cctz)              |
| double-conversion | >= 3.0   | BSD-3-Clause | [github](https://github.com/google/double-conversion) |
| flatbuffers       | >= 1.7   | Apache-2.0   | [github](https://github.com/google/flatbuffers)       |
| gflags            | >= 2.2   | BSD-3-Clause | [github](https://github.com/gflags/gflags)            |
| glog              | >= 0.3.5 | BSD-3-Clause | [github](https://github.com/google/glog)              |
| googletest        | >= 1.8   | BSD-3-Clause | [github](https://github.com/google/googletest)        |
| libconfig         | >= 1.7   | LGPL-2.1     | [github](https://github.com/hyperrealm/libconfig)     |
| libevent          | >= 2.1   | BSD-3-Clause | [github](https://github.com/libevent/libevent)        |

### Building

    ./autogen.sh
    ./configure
    make
    make install

### Broker API's

The gateways have not been open sourced.
You can only download pre-built binaries from the Conda repository (see next section).

*Please note that gateways will only work with valid license keys*.

### Conda

You are able to access pre-built binaries from the Conda repository: <http://quinclas.com/dist/conda/stable>.

Here is an example of how to set up your own Conda environment, install a package, and use a binary.

    # download the installer
    wget -c http://repo.continuum.io/miniconda/Miniconda2-latest-Linux-x86_64.sh

    # install miniconda
    bash ./Miniconda2-latest-Linux-x86_64.sh -b -p ~/miniconda2

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
    example-trading-engine

For further details about Conda, please refere to the official documentation ([link](https://conda.io/docs/)).
