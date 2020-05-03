include(FindPackageHandleStandardArgs)

find_path(LIBSENSORS_INCLUDE_DIRS
  NAMES sensors/sensors.h)

find_library(LIBSENSORS_LIBRARIES
  NAMES sensors)

find_package_handle_standard_args(LibSensors
  REQUIRED_VARS
    LIBSENSORS_LIBRARIES
    LIBSENSORS_INCLUDE_DIRS)

mark_as_advanced(LIBSENSORS_INCLUDE_DIRS)
mark_as_advanced(LIBSENSORS_LIBRARIES)

if(NOT TARGET libsensors::libsensors)
  add_library(libsensors::libsensors INTERFACE IMPORTED)
  set_target_properties(libsensors::libsensors PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${LIBSENSORS_INCLUDE_DIRS}"
    INTERFACE_LINK_LIBRARIES "${LIBSENSORS_LIBRARIES}")
endif()
