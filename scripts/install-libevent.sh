#!/usr/bin/env bash
set -ex
VERSION="release-2.1.8-stable"
wget https://github.com/libevent/libevent/archive/$VERSION.tar.gz
tar -xzvf $VERSION.tar.gz
cd libevent-$VERSION && cmake -DCMAKE_BUILD_TYPE=Release . && make -j2 && sudo make install
