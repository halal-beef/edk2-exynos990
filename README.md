Attempt to create a minimal EDK2 for Exynos 9830 devices

## Status
Boots to UEFI Shell.

### Building
Tested on Ubuntu 22.04.

First, clone EDK2.

```
cd ..
git clone https://github.com/tianocore/edk2 --recursive --depth=1
git clone https://github.com/tianocore/edk2-platforms.git
```

You should have all three directories side by side.

Next, install dependencies:

22.04:

```
sudo apt install build-essential uuid-dev iasl git nasm python3-distutils gcc-aarch64-linux-gnu
```

Also see [EDK2 website](https://github.com/tianocore/tianocore.github.io/wiki/Using-EDK-II-with-Native-GCC#Install_required_software_from_apt)

Run ```sh 
./firstrun.sh``` to setup the environment for development.

Then run ```sh
./build.sh``` to then initiate the build.

This should make a boot.tar image to be flashed in ODIN, you can also use the boot.img produced in heimdall as well.

## Credits

SimpleFbDxe screen driver is from imbushuo's [Lumia950XLPkg](https://github.com/WOA-Project/Lumia950XLPkg).

Zhuowei for making edk2-pixel3

All the people in ``EDKII pain and misery, struggles and disappointment`` on Discord.
