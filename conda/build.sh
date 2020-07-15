#!/usr/bin/env bash

set -e

ROQ_USE_CMAKE=1

env

if [ -z ${ROQ_USE_CMAKE+x} ]; then

export CPPFLAGS="-I$PREFIX/include -DNDEBUG -fvisibility=hidden $CPPFLAGS"
export LDFLAGS="-L$PREFIX/lib $LDFLAGS"
export PKG_CONFIG_PATH="$PREFIX/lib/pkgconfig"

./autogen.sh

./configure --prefix=$PREFIX --enable-doxygen

make -j${CPU_COUNT:-2}

make install-strip

else

cmake \
  -DCMAKE_AR="${CMAKE_AR:-$AR}" \
  -DCMAKE_RANLIB="${CMAKE_RANLIB:-$RANLIB}" \
  -DCMAKE_NM="${CMAKE_NM:-$NM}" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX:PATH=$PREFIX \
  -DBUILD_TESTING=ON \
  .

make VERBOSE=1 -j${CPU_COUNT:-2}
make install

fi
