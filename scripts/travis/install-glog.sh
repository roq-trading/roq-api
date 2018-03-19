#!/usr/bin/env bash
set -ex
rm -rf glog-master.zip
wget --content-disposition https://codeload.github.com/google/glog/zip/master
unzip glog-master.zip
cd glog-master
cmake -DCMAKE_BUILD_TYPE=Release .
make -j2
sudo make install
