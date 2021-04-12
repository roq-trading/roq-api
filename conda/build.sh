#!/usr/bin/env bash

set -e

CXXFLAGS+=" $CPPFLAGS"  # CMake doesn't used CPPFLAGS
CXXFLAGS+=" -Wno-error=unused-result"  # gtest doesn't check fwrite return value

echo -e "\033[1;34m--- ENV ---\033[0m"

env | sort

echo -e "\033[1;34m--- CMAKE ---\033[0m"

cmake \
  ${CMAKE_ARGS} \
  -DCMAKE_INSTALL_PREFIX:PATH=$PREFIX \
  -DBUILD_TESTING=ON \
  -DBUILD_DOCS=ON \
  .

echo -e "\033[1;34m--- MAKE ---\033[0m"

make VERBOSE=1 -j${CPU_COUNT:-2}
make install

echo -e "\033[1;34m--- DONE ---\033[0m"
