# roq-api

API for algorithmic and high-frequency trading (HFT).

> Note!
> This project does **not** contain the implementation of the C++ interfaces.
> This is due to the ultra low latency communication protocol being closed
> source.


## Features

* Open source interface (no need to sign an NDA to access or use).
* Permissive license (anyone is free to copy and use for whatever purpose).
* Unified interface for all markets.
* Design is strongly inspired by standards and specific implementations used
  by major exchanges.
* Allocation-free message decoding.
* Auto-generated code based on schemas.
* Strongly typed messages (events).
* Asynchronous interfaces and implementations.
* C++ for ultra low latency.
* FlatBuffers as an optional interface when latency is not a concern.


## Operating Systems

* Linux (x86-64, AArch64)
* macOS (x86-64)


## Library/Package Dependencies

* [FlatBuffers](https://github.com/google/flatbuffers) (Apache 2.0 License)
* [fmt](https://github.com/fmtlib/fmt) (MIT License)
* [range-v3](https://github.com/ericniebler/range-v3) (BSL 1.0 License)
* [span-lite](https://github.com/martinmoene/span-lite) (BSL 1.0 License)

Optional

* [Google Test](https://github.com/google/googletest) (BSD 3-Clause License)


## Prerequisites

The project is primarily designed to be compatible with the conda package manager.

### Download and Install Miniforge

#### Linux / x86\_64

```bash
wget -N https://github.com/conda-forge/miniforge/releases/latest/download/Miniforge3-Linux-x86_64.sh

bash Miniforge3-Linux-x86_64.sh -b -u -p ~/conda

~/conda/bin/conda install -y \
    'gxx_linux-64>=11'
```

#### Linux / aarch64

```bash
wget -N https://github.com/conda-forge/miniforge/releases/latest/download/Miniforge3-Linux-aarch64.sh

bash Miniforge3-Linux-aarch64.sh -b -u -p ~/conda

~/conda/bin/conda install -y \
    'gxx_linux-aarch64>=11'
```

#### macOS / x86\_64

```bash
wget -N https://github.com/conda-forge/miniforge/releases/latest/download/Miniforge3-MacOSX-x86_64.sh

bash Miniforge3-MacOSX-x86_64.sh -b -u -p ~/conda

~/conda/bin/conda install -y \
   clang_osx-64
```

### Install Dependencies

```bash
source ~/conda/bin/activate

conda install -y \
    git \
    cmake \
    flatbuffers \
    fmt

conda install -y --channel https://roq-trading.com/conda/stable \
    roq-oss-range-v3 \
    roq-oss-span-lite
```

In addition, your system should provide `clang-format` (at least version 10).


## Building

```bash
git submodule update --init --recursive

cmake .

make -j4

make test
```


## Using

Documentation can be found [here](https://roq-trading.com/docs).

### C++

Although the API is open source, certain solutions are closed source.

You can download the client library like this

```bash
conda install -y --channel https://roq-trading.com/conda/stable \
    roq-client
```

Samples can be found [here](https://github.com/roq-trading/roq-samples).

![C++ design](/doc/images/roq-api-cpp-design.png)

### FlatBuffers

FlatBuffers is primarily used for the event-log storage format.

![FlatBuffers design](/doc/images/roq-api-flatbuffers-design.png)


## License

The project is released under the terms of the MIT license.


## Links

* [Roq GmbH](https://roq-trading.com/)
* [Documentation](https://roq-trading.com/docs/)
* [Issues](https://github.com/roq-trading/roq-issues/issues)
* [Contact us](mailto:info@roq-trading.com)
