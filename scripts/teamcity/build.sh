#!/usr/bin/env bash

set -e

ROQ_TRADING_CONDA_REPO="${ROQ_TRADING_CONDA_REPO:-https://roq-trading.com/conda/unstable}"

echo "Prepare the Conda environment..."

curl -s https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh > ./miniconda.sh
bash ./miniconda.sh -b -p miniconda -u
cat > miniconda/.condarc << EOF
channels:
  - anaconda
  - $ROQ_TRADING_CONDA_REPO
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
  roq-oss-gflags \
  roq-oss-libevent \
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
make
make check
make dist

echo "Done!"
