#!/bin/sh
set -e
. ./iso.sh

qemu-system-i386 -rtc base=localtime -cdrom freemicronix.iso
