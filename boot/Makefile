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

all: help

.PHONY: all help img qemu

help:
	@echo "FreeMicroBoot Makefile"
	@echo ""
	@echo "Options:"
	@echo ""
	@echo "img   - creates a floppy image of the bootloader"
	@echo "qemu  - runs the bootloader floppy in qemu"
	@echo "clean - cleans up floppy image and object files"

img: stage1/stage1.asm
	nasm -f bin -o stage1.img stage1/stage1.asm

qemu: img
	qemu-system-i386 -fda stage1.img

clean:
	rm -rf stage1.img