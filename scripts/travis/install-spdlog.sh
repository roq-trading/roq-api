#!/usr/bin/env bash
set -ex
VERSION="1.0.0"
rm -rf "spdlog-$VERSION.zip"
wget --content-disposition "https://github.com/gabime/spdlog/archive/v$VERSION.zip"
unzip "spdlog-$VERSION.zip"
cd "spdlog-$VERSION"
cmake -DCMAKE_BUILD_TYPE=Release .
make -j2
sudo make install
