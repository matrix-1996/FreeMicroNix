#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/freemicronix.kernel isodir/boot/myos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "freemicronix" {
	multiboot /boot/freemicronix.kernel
}
EOF
grub-mkrescue -o freemicronix.iso isodir
