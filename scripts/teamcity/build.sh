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

source miniconda/bin/activate

conda install --quiet -y \
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

echo "Build ..."

env

./autogen.sh
./configure --prefix=$CONDA_PREFIX
make -j4

echo "Done!"
