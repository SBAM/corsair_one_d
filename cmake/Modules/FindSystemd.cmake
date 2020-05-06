include(FindPackageHandleStandardArgs)

pkg_check_modules(PkgConfSystemd systemd)

if(PkgConfSystemd_FOUND)
  execute_process(
    COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=systemdsystemunitdir systemd
    OUTPUT_VARIABLE SYSTEMD_SYSTEM_UNIT_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

find_package_handle_standard_args(Systemd
  REQUIRED_VARS
    SYSTEMD_SYSTEM_UNIT_DIR)

mark_as_advanced(SYSTEMD_SYSTEM_UNIT_DIR)

if(NOT SYSTEMD_SYSTEM_UNIT_DIR)
  message(WARNING "systemd's sytem units directory not found, won't install")
endif()
