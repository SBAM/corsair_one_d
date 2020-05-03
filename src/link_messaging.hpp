#ifndef CORSAIR_ONE_D_LINK_MESSAGING_HPP_
# define CORSAIR_ONE_D_LINK_MESSAGING_HPP_

# include "libusb_wrappers.hpp"

namespace cod
{

  namespace details
  {

    /// @brief Regroups devices details for CS-9000015-eu
    struct defs_cs_9000015_eu
    {
      static constexpr std::uint16_t vendor_id = 0x1b1c;
      static constexpr std::uint16_t product_id = 0x0c04;
      static constexpr std::uint8_t device_id = 0x50;
      static constexpr std::uint8_t cmd_id_init = 0x81;
      static constexpr std::uint8_t cmd_read_byte = 0x07;
      static constexpr std::uint8_t cmd_read_short = 0x09;
      static constexpr std::uint8_t cmd_write_byte = 0x06;
      static constexpr std::uint8_t cmd_device_id = 0x00;
      static constexpr std::uint8_t cmd_temp_sensor_count = 0x0d;
      static constexpr std::uint8_t cmd_select_sensor = 0x0f;
      static constexpr std::uint8_t cmd_sensor_temp = 0x0e;
      static constexpr std::uint8_t cmd_fan_count = 0x11;
      static constexpr std::uint8_t cmd_select_fan = 0x10;
      static constexpr std::uint8_t cmd_fan_mode = 0x12;
      static constexpr std::uint8_t cmd_fan_rpm = 0x16;
      static constexpr std::uint8_t cmd_fan_max_rpm = 0x17;
      static constexpr std::uint8_t cmd_fan_mode_fixed_pwm = 0x02;
      static constexpr std::uint8_t cmd_fan_fixed_pwm = 0x13;
      static std::uint8_t cmd_id;
    };

  } // !namespace details

  /// Default device definitions, leaves room for later improvements
  using defs = details::defs_cs_9000015_eu;

  /// @return true if handle's device ID matches defs'
  template <typename T>
  bool check_device_id(const libusb_dev_hdl_uptr& dev_hdl);

  /// @return true if device has a temperature sensor
  template <typename T>
  bool check_has_temp_sensor(const libusb_dev_hdl_uptr& dev_hdl);

  /// @return coolant temperature
  template <typename T>
  std::optional<double> get_coolant_temp(const libusb_dev_hdl_uptr& dev_hdl);

  /// @brief set top fan to max speed
  template <typename T>
  void set_top_fan_max_speed(const libusb_dev_hdl_uptr& dev_hdl);

} // !namespace cod

# include "link_messaging.ipp"

#endif // !CORSAIR_ONE_D_LINK_MESSAGING_HPP_
