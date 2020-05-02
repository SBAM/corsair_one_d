file(GLOB HEADERS *.hpp *.ipp)
file(GLOB SOURCES *.cpp)

add_executable(corsair_one_d
  ${HEADERS}
  ${SOURCES})

set_property(TARGET corsair_one_d
  PROPERTY POSITION_INDEPENDENT_CODE ON)

target_compile_definitions(corsair_one_d
  PRIVATE
    BOOST_ASIO_NO_DEPRECATED)

target_link_libraries(corsair_one_d
  PRIVATE
    Boost::system
    PkgConfig::libusb
    Threads::Threads)
