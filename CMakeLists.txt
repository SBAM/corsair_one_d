# CMP0083 PIE policy introduced in 3.14
cmake_minimum_required(VERSION 3.14)

project(corsair_one_d
  DESCRIPTION "CorsairOne daemon"
  LANGUAGES CXX)

message("-- cmake toolchain file = ${CMAKE_TOOLCHAIN_FILE}")

# select C++20 as standard
set(CMAKE_CXX_STANDARD 20)

# check position independent support (-fPIC)
include(CheckPIESupported)
check_pie_supported(OUTPUT_VARIABLE PIE_MSG
  LANGUAGES CXX)
if (NOT CMAKE_CXX_LINK_PIE_SUPPORTED)
  message(FATAL_ERROR "Linker with PIE capabilities required: ${PIE_MSG}")
endif()

# Searches for dependencies
# - pkg-config
find_package(PkgConfig REQUIRED)
# - Boost
find_package(Boost REQUIRED COMPONENTS system)
# - libusb
pkg_check_modules(libusb REQUIRED IMPORTED_TARGET libusb-1.0)
# - Threads
find_package(Threads REQUIRED)

# Provide project directory
add_subdirectory(src/)
