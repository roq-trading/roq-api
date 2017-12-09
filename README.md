# Quinclas - programmatic access to broker API's.

Copyright (c) 2017, Hans Erik Thrane

## Overview

This repository contains the public interfaces.

![overview](https://github.com/quinclas/tradingapi/blob/gh-pages/_images/design.png)

Detailed documentation available [here](https://quinclas.github.io/tradingapi/index.html).

## Usage

### Requirements

    autoconf
    automake
    libtool
    gcc >= 4.8.1
    clang >= 3.3
    glog >= 0.3.5
    gflags >= 2.2
    flatbuffers >= 1.7
    libevent >= 2.1
    cctz
    double-conversion

### Building

    ./autogen.sh
    ./configure
    make
    make install
