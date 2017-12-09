#!/usr/bin/env bash
set -ex
sudo apt-get -qq update
sudo apt-get install -y build-essential m4 autoconf autoconf-archive automake libtool pkg-config cmake gcc libgflags-dev libgoogle-glog-dev libevent-dev libconfig++-dev
