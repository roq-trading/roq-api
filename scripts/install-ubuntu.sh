#!/usr/bin/env bash
set -ex
sudo apt-get -qq update
# note - libgflags-dev is too old, so not installing from apt
sudo apt-get install -y build-essential m4 autoconf autoconf-archive automake libtool pkg-config cmake gcc libgoogle-glog-dev libevent-dev libconfig++-dev
