all: floppy

.PHONY: all clean floppy iso qemu qemu-noroot

clean:
	rm -f boot.bin
	rm -f boot.img
	rm -f boot.iso
	rm -rf cdiso

floppy: clean boot0.asm 
	nasm -f bin -o boot.bin boot0.asm
	dd bs=512 count=2880 if=/dev/zero of=boot.img
	dd status=noxfer conv=notrunc if=boot.bin of=boot.img

iso: clean floppy
	mkdir -p cdiso
	cp boot.img cdiso/
	mkisofs -pad -b boot.img -R -o boot.iso cdiso/

qemu: iso
	qemu-system-i386 -cdrom boot.iso

qemu-noroot: floppy
	qemu-system-i386 -fda boot.img
