Roq's Flatbuffers API

We don't yet use `--cpp-std c++17` with `flatc`.

Reason is that this command-line flag wasn't available with flatbuffers
version 1.11.0, the default shipped with Ubuntu 20.04 LTS.

We intend to upgrade the API when Ubuntu 24.04 LTS becomes available.
