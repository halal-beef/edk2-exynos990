#!/bin/bash
set -e

sudo apt update
sudo apt install -y build-essential uuid-dev iasl git nasm gcc-aarch64-linux-gnu bc

curdir="$PWD"

wget https://raw.githubusercontent.com/halal-beef/res/main/mkbootimg
chmod +x mkbootimg

export PATH=$PATH:$curdir

cd ..

git clone https://github.com/tianocore/edk2 --recursive --depth=1
git clone https://github.com/tianocore/edk2-platforms.git

cd "$curdir"

./firstrun.sh
./build.sh
