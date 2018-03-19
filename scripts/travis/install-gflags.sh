#!/usr/bin/env bash
set -ex
rm -rf gflags-master.zip
wget --content-disposition https://codeload.github.com/gflags/gflags/zip/master
unzip gflags-master.zip
cd gflags-master
cmake -DCMAKE_BUILD_TYPE=Release .
make -j2
sudo make install
