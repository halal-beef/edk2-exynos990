#!/bin/bash
# based on the instructions from edk2-platform
set -e
. build_common.sh
# not actually GCC5; it's GCC7 on Ubuntu 18.04.
./build_bootshim.sh
GCC5_AARCH64_PREFIX=aarch64-linux-gnu- build -j$(nproc --all) -s -n 0 -a AARCH64 -t GCC5 -p EXYNOS7885Pkg/Devices/s20.dsc
cat BootShim/BootShim.bin workspace/Build/EXYNOS7885Pkg/DEBUG_GCC5/FV/EXYNOS7885PKG_UEFI.fd > workspace/UEFI
mkbootimg --tags_offset 0x00000100 --second_offset 0xf0000000 --ramdisk_offset 0x01000000 --pagesize 2048 --os_version "13.0.0" --os_patch_level "2023-07" --kernel_offset 0x00008000 --header_version 2  --dtb_offset 0x00000000 --cmdline "buildvariant=userdebug" --board "" --base 0x10000000 --ramdisk workspace/dummyramdisk --kernel workspace/UEFI --dtb workspace/dtb -o workspace/boot.img
cd workspace/
tar -c boot.img -f boot.tar
cd ..
