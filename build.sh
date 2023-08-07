#!/bin/bash
# based on the instructions from edk2-platform
set -e
. build_common.sh
# not actually GCC5; it's GCC7 on Ubuntu 18.04.
./build_bootshim.sh
GCC5_AARCH64_PREFIX=aarch64-linux-gnu- build -s -n 0 -a AARCH64 -t GCC5 -p MSM8916Pkg/Devices/a10.dsc
cat BootShim/BootShim.bin workspace/Build/MSM8916Pkg/DEBUG_GCC5/FV/MSM8916PKG_UEFI.fd > workspace/bootOG.img-kernel

