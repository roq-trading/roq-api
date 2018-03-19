#!/usr/bin/env bash
set -ex
rm -rf crossguid-master.zip
wget --content-disposition https://codeload.github.com/graeme-hill/crossguid/zip/master
unzip crossguid-master.zip
cd crossguid-master
cmake -DCMAKE_BUILD_TYPE=Release .
make -j2
sudo make install
