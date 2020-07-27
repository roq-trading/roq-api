# roq-api


The client interface used to communicate with market gateways and simulators.

Features

* Open sourced interface (no need to sign an NDA).
* Permissive license (anyone is free to copy and use for whatever purpose).
* Unified interface for all markets.
* Design is strongly inspired by standards (e.g. FIX) and implementations used
  by major exchanges (e.g. CME).
* Allocation-free message decoding supported.
* Auto-generated code.
* Strongly typed events.
* fmt support.
* c++ interface for ultra low latency.
* flatbuffers interface for non latency sensitive applications.
* fmt support (c++).

> Note! This project does **not** contain the c++ implementation of the
> interfaces. The primary reason is that the implementation of the ultra low
> latency communication protocol is closed source.

Direct third-party dependencies

* [flatbuffers](https://github.com/google/flatbuffers) (Apache 2.0 License)
* [fmt](https://github.com/fmtlib/fmt) (MIT License)


## Operating Systems

* Linux


## Prerequisites

The project is designed to be compatible with the conda package manager.

This is one way to create a conda environment and install the required
packages

```bash
wget -N https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh

bash Miniconda3-latest-Linux-x86_64.sh -b -u -p ~/miniconda3

source ~/miniconda3/bin/activate

conda install -y \
    git \
    cmake \
    gxx_linux-64

conda install -y --channel https://roq-trading.com/conda/stable \
    roq-oss-flatbuffers \
    roq-oss-fmt
```


## Building

```bash
git submodule update --init --recursive

cmake .

make -j4

make test
```


## Using

### c++

Using the c++ interface requires a closed source implementation

```bash
conda install -y --channel https://roq-trading.com/conda/stable \
    roq-client
```

Samples can be found [here](https://github.com/roq-trading/roq-samples).

### flatbuffers

Clients can use the flatbuffers interface when latency is not a concern.
The flatbuffers project supports a variety of programming languages.

Use cases

* A non latency sensitive interface supported by all market gateways.
* The storage format used when persisting an event log (tick data).

#### [Python](./python)

> WORK IN PROGRESS.


## Links

* [Documentation](https://roq-trading.com/docs)
* [Contact us](mailto:info@roq-trading.com)
