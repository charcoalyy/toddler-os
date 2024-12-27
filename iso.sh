#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/toddleros.kernel isodir/boot/toddleros.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "toddleros" {
	multiboot /boot/toddleros.kernel
}
EOF
grub-mkrescue -o toddleros.iso isodir
