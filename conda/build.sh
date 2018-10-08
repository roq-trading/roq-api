#!/usr/bin/env bash

set -e

env

export CPPFLAGS="-I$PREFIX/include $CPPFLAGS"
export LDFLAGS="-L$PREFIX/lib $LDFLAGS"
export PKG_CONFIG_PATH="$PREFIX/lib/pkgconfig"

./autogen.sh
./configure \
  --prefix=$PREFIX \
  --enable-benchmark
make -j${CPU_COUNT:-2}
make check
make install
