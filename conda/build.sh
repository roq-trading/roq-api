#!/usr/bin/env bash

set -e

case "$ARCH" in
  64)
    CFLAGS="${CFLAGS/-march=nocona/-march=broadwell}"
    CFLAGS="${CFLAGS/-march=core2/-march=broadwell}"
    CFLAGS="${CFLAGS/-mtune=haswell/-mtune=broadwell}"
    CXXFLAGS="${CXXFLAGS/-march=nocona/-march=broadwell}"
    CXXFLAGS="${CXXFLAGS/-march=core2/-march=broadwell}"
    CXXFLAGS="${CXXFLAGS/-mtune=haswell/-mtune=broadwell}"
    ;;

  arm64)
    ;;

  aarch64)
    ;;

  *)
    (>&2 echo -e "\033[1;31mERROR: Unsupported ARCH=$ARCH.\033[0m") && exit 1
    ;;
esac

CFLAGS="$CFLAGS -O3"
CPPFLAGS="$CPPFLAGS -O3"

CXXFLAGS+=" $CPPFLAGS"  # CMake doesn't used CPPFLAGS

echo -e "\033[1;34m--- ENV ---\033[0m"

env | sort

echo -e "\033[1;34m--- CMAKE ---\033[0m"

cmake \
  ${CMAKE_ARGS} \
  -DCMAKE_INSTALL_PREFIX:PATH=$PREFIX \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_TESTING=ON \
  -DBUILD_DOCS=OFF \
  .

echo -e "\033[1;34m--- MAKE ---\033[0m"

make VERBOSE=1 -j${CPU_COUNT:-2}
make install

echo -e "\033[1;34m--- DONE ---\033[0m"
