# CorsairOne daemon

If `coretemp` (max temperature across all CPU cores) or `coolant temperature` (liquid circuit temperature) exceeds pre-defined thresholds, the daemon requests internal _CorsairOne_'s USB device to increase fan speed.

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
```console
user@mach:~$ git clone https://github.com/SBAM/corsair_one_d.git
user@mach:~$ cd corsair_one_d
user@mach:~$ ./configure.sh
user@mach:~$ make -C build/Release
user@mach:~$ sudo make -C build/Release install
## reload systemd's configuration
user@mach:~$ sudo systemctl daemon-reload
## simulate corsair's USB device connected
user@mach:~$ sudo udevadm trigger --attr-match=subsystem=usb --attr-match=idVendor=1b1c --action=add
## check systemd service succesfully started
user@mach:~$ journalctl -f
[...] systemd[1]: Started CorsairOne daemon.
[...] audit[1]: SERVICE_START [...] msg='unit=CorsairOneDaemon comm="systemd" exe="/usr/lib/systemd/systemd" hostname=? addr=? terminal=? res=success'
```

## Acknowledgments
Many thanks to:
 * *audiohacked* for [OpenCorsairLink](https://github.com/audiohacked/OpenCorsairLink) who reverse engineered CorsairLink protocol
 * *CFSworks* for his [investigations](https://forum.corsair.com/v3/showthread.php?t=120092) on CorsairLink protocol




