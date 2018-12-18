#!/usr/bin/env bash

set -e

env

export CPPFLAGS="-I$PREFIX/include -DNDEBUG $CPPFLAGS"
export LDFLAGS="-L$PREFIX/lib $LDFLAGS"
export PKG_CONFIG_PATH="$PREFIX/lib/pkgconfig"

./autogen.sh
./configure \
  --prefix=$PREFIX \
  --enable-benchmark \
  --enable-doc
make -j${CPU_COUNT:-2}
make check
make install
