#Copyright (C) <2016>  William Taylor Jones <freemicronix@gmail.com>
#
#This program is free software; you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation; either version 2 of the License, or
#(at your option) any later version.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License along
#with this program; if not, write to the Free Software Foundation, Inc.,
#51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#!/bin/bash
set -ex

mkdir -p toolchain/src

# Set the software versions
export BINVER='2.27'
export GCCVER='6.2.0'

# Set the env variables
export PREFIX=$(pwd)/tools
export TARGET='i686-elf'
export PATH="$PREFIX/i686-elf":$PATH

# Get the source tarballs
cd toolchain/src
wget ftp://ftp.gnu.org/gnu/binutils/binutils-$BINVER.tar.gz
wget ftp://ftp.gnu.org/gnu/gcc/gcc-$GCCVER/gcc-$GCCVER.tar.gz

# Extract the souces
tar -xf binutils-$BINVER.tar.gz
tar -xf gcc-$GCCVER.tar.gz

mkdir -p build-binutils
mkdir -p build-gcc


# Setup the build dirs


cd build-binutils
../binutils-$BINVER/configure --target=$TARGET --prefix=$PREFIX --with-sysroot --disable-nls --disable-werror
make
make install

cd ../gcc-$GCCVER
contrib/download_prerequisites

cd ../build-gcc
../gcc-$GCCVER/configure --target=$TARGET --prefix=$PREFIX --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc

echo "Cross Toolchain Built"
