# Quinclas - programmatic access to broker API's.

Copyright (c) 2017, Hans Erik Thrane

## Overview

This repository contains the public interfaces.

![overview](https://github.com/quinclas/tradingapi/blob/gh-pages/_images/design.png)

Detailed documentation available [here](https://quinclas.github.io/tradingapi/index.html).

## Usage

### Platforms

    Linux
    OSX

### Build Tools

    autoconf
    automake
    libtool
    gcc >= 4.8.1
    clang >= 3.3

### Dependencies

    glog >= 0.3.5 (BSD 3-Clause)
    gflags >= 2.2 (BSD 3-Clause)
    libconfig >= 1.7  (LGPL 2.1)
    flatbuffers >= 1.7 (Apache 2.0)
    libevent >= 2.1 (BSD 3-Clause)
    cctz >= 2.1 (Apache 2.0)
    double-conversion >= 3.0 (BSD 3-Clause)

### Building

    ./autogen.sh
    ./configure
    make
    make install
