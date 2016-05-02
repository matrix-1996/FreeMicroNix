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
	@echo "iso - builds a bootable .iso file of FreeMicroNix"
	@echo "qemu - runs FreeMicroNix in qemu"
	@echo "qemu-enable-kvm - runs FreeMicroNix in qemu with the switch -enable-kvm"
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


iso: build
	mkdir -p isodir
	mkdir -p isodir/boot
	mkdir -p isodir/boot/grub

	cp sysroot/boot/freemicronix.kernel isodir/boot/freemicronix.kernel
	cp grub.cfg isodir/boot/grub/grub.cfg

	grub-mkrescue -o freemicronix.iso isodir



qemu: iso
	qemu-system-i386 -rtc base=localtime -cdrom freemicronix.iso

qemu-enable-kvm: iso
	qemu-system-i386 -rtc base=localtime -enable-kvm -cdrom freemicronix.iso
