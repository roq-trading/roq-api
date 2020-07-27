# roq-api


The client interface used to communicate with market gateways and simulators.

Features

* Open source interface (no need to sign an NDA to access or use).
* Permissive license (anyone is free to copy and use for whatever purpose).
* Unified interface for all markets.
* Design is strongly inspired by standards and specific implementations used
  by major exchanges.
* Allocation-free message decoding supported.
* Auto-generated code based on schemas.
* Strongly typed messages (events).
* Asynchronous interfaces and implementations.
* c++ for ultra low latency.
* flatbuffers when latency is not a concern.

> Note! This project does **not** contain the implementation of the c++
> interfaces. This is due to the ultra low latency communication protocol
> being closed source.

Direct third-party dependencies

* [flatbuffers](https://github.com/google/flatbuffers) (Apache 2.0 License)
* [fmt](https://github.com/fmtlib/fmt) (MIT License)


## Operating Systems

* Linux


## Prerequisites

The project is designed to be compatible with the conda package manager.

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

> Please note that many of the interfaces are being auto-generated and
> therefore only accessible once the project has been built or installed.

### c++

The c++ interface is designed for ultra low latency and requires a closed
source implementation

```bash
conda install -y --channel https://roq-trading.com/conda/stable \
    roq-client
```

Samples can be found [here](https://github.com/roq-trading/roq-samples).

![c++ design](https://roq-trading.com/assets/roq-api-cpp-design.png)

### flatbuffers

The flatbuffers interface is useful when latency is not a concern or when
other programming languages than c++ are required.
Furthermore, the event log storage format uses flatbuffers and can be
decoded from any language.

![flatbuffers design](https://roq-trading.com/assets/roq-api-flatbuffers-design.png)

#### [Python](./python)

> WORK IN PROGRESS.


## License

The project is released under the terms of the MIT license.


## Links

* [Documentation](https://roq-trading.com/docs)
* [Contact us](mailto:info@roq-trading.com)
