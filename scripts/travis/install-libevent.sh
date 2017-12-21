#!/usr/bin/env bash
set -ex
wget --content-disposition https://codeload.github.com/libevent/libevent/zip/master
unzip libevent-master.zip
cd libevent-master
./autogen.sh
./configure
make -j2
sudo make install
