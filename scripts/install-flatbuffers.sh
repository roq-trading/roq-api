#!/usr/bin/env bash
set -ex
wget https://github.com/google/flatbuffers/archive/v1.8.0.tar.gz
tar -xzvf v1.8.0.tar.gz
cd flatbuffers-1.8.0 && cmake . && make -j2 && sudo make install
