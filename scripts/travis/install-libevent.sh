#!/usr/bin/env bash
set -ex

# rm -rf libevent-master.zip
# wget --content-disposition https://codeload.github.com/libevent/libevent/zip/master
# unzip libevent-master.zip
# cd libevent-master

NAME="release-2.1.8-stable"
wget "https://github.com/libevent/libevent/archive/$NAME.zip"
unzip "$NAME.zip"
cd "libevent-$NAME"

./autogen.sh
./configure
make -j2
sudo make install
