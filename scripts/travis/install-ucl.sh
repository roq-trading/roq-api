#!/usr/bin/env bash
set -ex
wget --content-disposition https://codeload.github.com/vstakhov/libucl/zip/master
unzip libucl-master.zip
cd libucl-master
./autogen.sh
./configure
make -j2
sudo make install
