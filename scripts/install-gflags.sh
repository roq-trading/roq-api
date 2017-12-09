#!/usr/bin/env bash
set -ex
VERSION="2.2.1"
wget https://github.com/gflags/gflags/archive/v$VERSION.tar.gz
tar -xzvf v$VERSION.tar.gz
cd gflags-$VERSION && cmake -DCMAKE_BUILD_TYPE=Release . && make -j2 && sudo make install
