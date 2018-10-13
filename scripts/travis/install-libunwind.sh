#!/usr/bin/env bash
set -ex
rm -rf libunwind-master.zip
wget --content-disposition https://codeload.github.com/libunwind/libunwind/zip/master
unzip libunwind-master.zip
cd libunwind-master
./autogen.sh
./configure
make -j2
sudo make install
