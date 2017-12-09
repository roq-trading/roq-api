#!/usr/bin/env bash
set -ex
wget --content-disposition https://codeload.github.com/google/cctz/zip/master
unzip cctz-master.zip
cd cctz-master
make j2
sudo make install
