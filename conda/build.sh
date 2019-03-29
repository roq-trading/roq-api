#!/usr/bin/env bash

set -e

env

export CPPFLAGS="-I$PREFIX/include -DNDEBUG -fvisibility=hidden $CPPFLAGS"
export LDFLAGS="-L$PREFIX/lib $LDFLAGS"
export PKG_CONFIG_PATH="$PREFIX/lib/pkgconfig"

./autogen.sh
./configure --prefix=$PREFIX
make -j${CPU_COUNT:-2}
set -e
make check || EXIT_CODE=$?
set +e
cat test/roq-*.log
if [ "$EXIT_CODE" -ne 0 ]; then
	exit $EXIT_CODE
fi
make install-strip
