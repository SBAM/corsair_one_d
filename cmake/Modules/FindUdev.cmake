include(FindPackageHandleStandardArgs)

pkg_check_modules(PkgConfUdev udev)

if(PkgConfUdev_FOUND)
  execute_process(
    COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=udevdir udev
    OUTPUT_VARIABLE UDEV_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  string(CONCAT TMP_UDEV_RULES_DIR ${UDEV_DIR} "/rules.d")
  if(EXISTS ${TMP_UDEV_RULES_DIR})
    set(UDEV_RULES_DIR ${TMP_UDEV_RULES_DIR})
  endif()
endif()

find_package_handle_standard_args(Udev
  REQUIRED_VARS
    UDEV_RULES_DIR)

mark_as_advanced(UDEV_RULES_DIR)

if(NOT UDEV_RULES_DIR)
  message(WARNING "udev rules directory not found, won't install")
endif()
