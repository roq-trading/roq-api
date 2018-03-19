#!/usr/bin/env bash
set -ex
wget --content-disposition https://codeload.github.com/gabime/spdlog/zip/master
unzip spdlog-master.zip
cd spdlog-master
cmake -DCMAKE_BUILD_TYPE=Release .
make -j2
sudo make install
