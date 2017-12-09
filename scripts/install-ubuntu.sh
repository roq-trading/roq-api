#!/usr/bin/env bash
set -ex
sudo apt-get -qq update
sudo apt-get install -y \
	libssl-dev build-essential m4 autoconf autoconf-archive automake \
       	libtool pkg-config cmake gcc libgoogle-glog-dev libconfig++-dev
