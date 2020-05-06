if(SYSTEMD_SYSTEM_UNIT_DIR AND UDEV_RULES_DIR)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CorsairOneDaemon.service.cmake
    ${PROJECT_BINARY_DIR}/CorsairOneDaemon.service
    @ONLY)
  install(FILES ${PROJECT_BINARY_DIR}/CorsairOneDaemon.service
    DESTINATION ${SYSTEMD_SYSTEM_UNIT_DIR})
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/99-corsair-one-d.rules
    DESTINATION ${UDEV_RULES_DIR})
endif()
