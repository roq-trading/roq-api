#!/usr/bin/env bash
set -ex
VERSION="1.9.0"
rm -rf "flatbuffers-$VERSION.zip"
wget --content-disposition "https://github.com/google/flatbuffers/archive/v$VERSION.zip"
unzip "flatbuffers-$VERSION.zip"
cd "flatbuffers-$VERSION"
cmake -DCMAKE_BUILD_TYPE=Release .
make -j2
sudo make install
