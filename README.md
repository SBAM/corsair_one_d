# CorsairOne daemon

Simple daemon that manages CorsairOne fan.
Upon installation:
* a custom *udev* rule is added to `/usr/lib/udev/rules.d/` (`pkg-config udev -> udevdir`)
* a custom *systemd unit* is added to `/usr/lib/systemd/system` (`pkg-config systemd -> systemdsystemunitdir`)
* daemon is deployed in [GNU standard bin directory](https://cmake.org/cmake/help/latest/module/GNUInstallDirs.html)

## Supported devices
* CS\-9000015\-EU

## Dependencies
* cxx20 compiler such as [g++](https://gcc.gnu.org/) or [clang++](https://clang.llvm.org/)
* [cmake](https://cmake.org/)
* [pkg\-config](https://www.freedesktop.org/wiki/Software/pkg-config/)
* [boost](https://www.boost.org/)
* [libusb](https://libusb.info/)
* [lm\_sensors](https://hwmon.wiki.kernel.org/lm_sensors)
* [spdlog](https://github.com/gabime/spdlog)

> Fedora 32 setup
```shell
$ dnf install cmake gcc-c++ pkg-config boost-devel libusb-devel lm_sensors-devel spdlog-devel
```

## Build and install
```shell
$ git clone https://github.com/SBAM/corsair_one_d.git
$ cd corsair_one_d
$ ./configure.sh
$ make -C build/Release
$ sudo make -C build/Release install
# reload systemd's configuration
$ sudo systemctl daemon-reload
# Signal udevd to reload the rules files
$ sudo udevadm control --reload-rules
# Request device events from the kernel for sysfs usb
$ sudo udevadm trigger --attr-match=subsystem=usb
```

## Acknowledgments
Many thanks to:
 * *audiohacked* for [OpenCorsairLink](https://github.com/audiohacked/OpenCorsairLink) who reverse engineered CorsairLink protocol
 * *CFSworks* for his [investigations](https://forum.corsair.com/v3/showthread.php?t=120092) on CorsairLink protocol




