#!/usr/bin/env bash
set -ex
VERSION="1.8.0"
wget https://github.com/google/flatbuffers/archive/v$VERSION.tar.gz
tar -xzvf v$VERSION.tar.gz
cd flatbuffers-$VERSION && cmake -DCMAKE_BUILD_TYPE=Release . && make -j2 && sudo make install
