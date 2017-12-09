#!/usr/bin/env bash
set -ex
wget https://github.com/gflags/gflags/archive/v2.2.1.tar.gz
tar -xzvf v2.2.1.tar.gz
cd gflags-2.2.1 && mkdir build && cd build && cmake .. && make -j2 && sudo make install
