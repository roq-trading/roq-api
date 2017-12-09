#!/usr/bin/env bash
set -ex
wget --content-disposition https://codeload.github.com/google/double-conversion/zip/master
unzip double-conversion-master.zip
cd double-conversion-master
cmake -DCMAKE_BUILD_TYPE=Release .
make -j2
sudo make install
