#!/usr/bin/env bash
set -ex
wget --content-disposition https://codeload.github.com/graeme-hill/crossguid/zip/master
unzip crossguid.zip
cd crossguid-master
cmake -DCMAKE_BUILD_TYPE=Release .
make -j2
sudo make install
