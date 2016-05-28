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


export HOST=$(PWD)/tools/bin/i686-elf
export AR=$(HOST)-ar
export AS=nasm
export CC=$(HOST)-gcc --sysroot=$(PWD)/sysroot -isystem=/usr/include
export DESTDIR=$(PWD)/sysroot
export ARCH=i686
export ARCHDIR=$(ARCH)

all: help

.PHONY: all help clean build headers kernel libc iso qemu qemu-enable-kvm

help:
	@echo "FreeMicroNix Make System"
	@echo ""
	@echo "Options:"
	@echo ""
	@echo "build - builds an elf kernel file of FreeMicroNix"
	@echo "debug - builds an elf kernel file of FreeMicroNix with debugging symbols"
	@echo "iso - builds a bootable .iso file of FreeMicroNix"
	@echo "iso-debug - builds a bootable iso file of freemicronix with debugging symbols"
	@echo "qemu - runs FreeMicroNix in qemu"
	@echo "qemu-enable-kvm - runs FreeMicroNix in qemu with the switch -enable-kvm"
	@echo "qemu - runs FreeMicroNix in qemu with debugging symbols"
	@echo "qemu-enable-kvm - runs FreeMicroNix in qemu with the switch -enable-kvm with debugging symbols"
	@echo "clean - cleans up object files and removes the sysroot and isodir directories"

clean:
	$(MAKE) -C libc clean
	$(MAKE) -C kernel clean
	rm -rfv sysroot
	rm -rfv isodir
	rm -rfv freemicronix.iso


headers:
	mkdir -p sysroot
	$(MAKE) -C libc install-headers
	$(MAKE) -C kernel install-headers

build: headers
	$(MAKE) -C libc install
	$(MAKE) -C kernel install
	strip kernel/freemicronix.kernel -o kernel/freemicronix.kernel

debug: headers
	$(MAKE) -C libc install
	$(MAKE) -C kernel install



iso: build
	mkdir -p isodir
	mkdir -p isodir/boot
	mkdir -p isodir/boot/grub

	cp sysroot/boot/freemicronix.kernel isodir/boot/freemicronix.kernel
	cp grub.cfg isodir/boot/grub/grub.cfg

	grub-mkrescue -o freemicronix.iso isodir


iso-debug: debug


qemu: iso
	qemu-system-i386 -rtc base=localtime -cdrom freemicronix.iso

qemu-enable-kvm: iso
	qemu-system-i386 -rtc base=localtime -enable-kvm -cdrom freemicronix.iso

qemu-debug: iso-debug
	qemu-system-i386 -rtc base=localtime -cdrom freemicronix.iso

qemu-debug-enable-kvm: iso-debug
	qemu-system-i386 -rtc base=localtime -enable-kvm -cdrom freemicronix.iso
