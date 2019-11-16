#!/usr/bin/env bash

set -e

env

export CPPFLAGS="-I$PREFIX/include -DNDEBUG -fvisibility=hidden $CPPFLAGS"
export LDFLAGS="-L$PREFIX/lib $LDFLAGS"
export PKG_CONFIG_PATH="$PREFIX/lib/pkgconfig"

./autogen.sh

./configure --prefix=$PREFIX --enable-doxygen

make -j${CPU_COUNT:-2}

make install-strip
