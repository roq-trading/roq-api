#!/usr/bin/env bash
set -ex
sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-get -qq update
sudo apt-get install -qq g++-6
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-6 90
sudo apt-get install -y \
	libssl-dev build-essential m4 autoconf autoconf-archive automake \
       	libtool pkg-config cmake gcc python-pip libconfig++-dev libspdlog-dev
sudo pip install cpplint
