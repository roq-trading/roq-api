#!/usr/bin/env bash

set -e

echo "Prepare the Conda environment..."

curl -s https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh > ./miniconda.sh
bash ./miniconda.sh -b -p miniconda -u
cat > miniconda/.condarc << EOF
channels:
  - anaconda
  - https://roq-trading.com/conda/unstable
EOF
miniconda/bin/conda update --quiet --yes --name base conda
miniconda/bin/conda install --quiet --yes \
  autoconf \
  automake \
  libtool \
  pkgconfig \
  make \
  cmake \
  gcc_linux-64 \
  gxx_linux-64 \
  patchelf \
  openssl \
  gflags \
  libevent \
  roq-oss-benchmark \
  roq-oss-cctz \
  roq-oss-crossguid \
  roq-oss-flatbuffers \
  roq-oss-libucl \
  roq-oss-libunwind \
  roq-oss-spdlog

echo "Activate the Conda environment..."

source miniconda/bin/activate

export LDFLAGS="$LDFLAGS -L$CONDA_PREFIX/lib"
export CPPFLAGS="$CPPFLAGS -I$CONDA_PREFIX/include"
export PKG_CONFIG_PATH="$CONDA_PREFIX/lib/pkgconfig"

env

echo "Build ..."

./autogen.sh
./configure
make -j4
make check
# make install

echo "Done!"
