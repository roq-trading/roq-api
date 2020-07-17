#!/usr/bin/env bash

set -e

env | sort

CXXFLAGS+=" $CPPFLAGS"  # CMake doesn't used CPPFLAGS
CXXFLAGS+=" -Wno-error=unused-result"  # gtest doesn't check fwrite return value

cmake \
  -DCMAKE_AR="${CMAKE_AR:-$AR}" \
  -DCMAKE_RANLIB="${CMAKE_RANLIB:-$RANLIB}" \
  -DCMAKE_NM="${CMAKE_NM:-$NM}" \
  -DCMAKE_INSTALL_PREFIX:PATH=$PREFIX \
  -DBUILD_TESTING=ON \
  .

make VERBOSE=1 -j${CPU_COUNT:-2}
make install
