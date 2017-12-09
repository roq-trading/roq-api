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

*Indirectly requiring on CMake*.

### Dependencies

| tool              | version  | license      | source                                                |
| ----------------- | -------- | ------------ | ----------------------------------------------------- |
| cctz              | >= 2.1   | Apache 2.0   | [github](https://github.com/google/cctz)              |
| double-conversion | >= 3.0   | BSD 3-Clause | [github](https://github.com/google/double-conversion) |
| flatbuffers       | >= 1.7   | Apache 2.0   | [github](https://github.com/google/flatbuffers)       |
| gflags            | >= 2.2   | BSD 3-Clause | [github](https://github.com/gflags/gflags)            |
| glog              | >= 0.3.5 | BSD 3-Clause | [github](https://github.com/google/glog)              |
| googletest        | >= 1.8   | BSD 3-Clause | [github](https://github.com/google/googletest)        |
| libconfig         | >= 1.7   | LGPL 2.1     | [github](https://github.com/hyperrealm/libconfig)     |
| libevent          | >= 2.1   | BSD 3-Clause | [github](https://github.com/libevent/libevent)        |

### Building

    ./autogen.sh
    ./configure
    make
    make install
