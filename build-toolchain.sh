#!/bin/bash
set -ex

mkdir -p toolchain/src
mkdir -p toolchain/bin

# Set the software versions
export BINVER='2.26'
export GCCVER='5.3.0'
export THEADS='4'

# Set the env variables
export PREFIX=$(pwd)/tools
export TARGET='i686-elf'
export PATH="$PREFIX/i686-elf":$PATH

# Get the source tarballs
cd toolchain/src
wget ftp://ftp.gnu.org/gnu/binutils/binutils-$BINVER.tar.gz
wget ftp://ftp.gnu.org/gnu/gcc/gcc-5.3.0/gcc-$GCCVER.tar.gz

# Extract the souces
tar -xf binutils-$BINVER.tar.gz
tar -xf gcc-$GCCVER.tar.gz

mkdir -p build-binutils
mkdir -p build-gcc


# Setup the build dirs


cd build-binutils
../binutils-$BINVER/configure --target=$TARGET --prefix=$PREFIX --with-sysroot --disable-nls --disable-werror
make -j$THREADS
make install
cd ../gcc-$GCCVER
contrib/download_prerequisites

cd ../build-gcc
../gcc-$GCCVER/configure --target=$TARGET --prefix=$PREFIX --disable-nls --enable-languages=c,c++ --without-headers
make -j$THREADS all-gcc
make -j$THREADS all-target-libgcc
make install-gcc
make install-target-libgcc

echo "Cross Toolchain Built"