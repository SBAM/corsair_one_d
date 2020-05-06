# CMP0083 PIE policy introduced in 3.14
cmake_minimum_required(VERSION 3.14)

project(corsair_one_d
  DESCRIPTION "CorsairOne daemon"
  LANGUAGES CXX)

message("-- cmake toolchain file = ${CMAKE_TOOLCHAIN_FILE}")

# select C++20 as standard
set(CMAKE_CXX_STANDARD 20)

# check position independent code into executables support (-fPIE)
include(CheckPIESupported)
check_pie_supported(OUTPUT_VARIABLE PIE_MSG
  LANGUAGES CXX)
if (NOT CMAKE_CXX_LINK_PIE_SUPPORTED)
  message(FATAL_ERROR "Linker with PIE capabilities required: ${PIE_MSG}")
endif()

# expose GNU standard installation directories
include(GNUInstallDirs OPTIONAL)

# Add custom find_package search path
list(APPEND CMAKE_MODULE_PATH
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")

# Searches for dependencies
# - pkg-config
find_package(PkgConfig REQUIRED)
# - Boost
find_package(Boost REQUIRED COMPONENTS
  program_options
  system)
# - libusb
pkg_check_modules(libusb REQUIRED IMPORTED_TARGET libusb-1.0)
# - libsensors
find_package(LibSensors REQUIRED)
# - spdlog
find_package(spdlog REQUIRED)
# - systemd
find_package(Systemd)
# - Threads
find_package(Threads REQUIRED)
# - udev
find_package(Udev)

# Project sources
add_subdirectory(src/)
# systemd unit deploy
add_subdirectory(systemd/)
