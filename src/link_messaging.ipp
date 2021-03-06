namespace cod
{

  template <typename T>
  bool check_device_id(const libusb_dev_hdl_uptr& dev_hdl)
  {
    spdlog::debug("verifying device_id={:#04x}", T::device_id);
    lusb_msg_t buf;
    buf.fill(0);
    buf[0] = 3; // length
    buf[1] = T::cmd_id++; // command ID
    buf[2] = T::cmd_read_byte; // op-code
    buf[3] = T::cmd_device_id; // command data
    lusb_write(dev_hdl, buf);
    // retrieve command response
    buf.fill(0);
    auto rs = lusb_read(dev_hdl, buf);
    if (rs >= 3)
    {
      auto dev_id = buf[2];
      spdlog::debug("got device_id={:#04x}", dev_id);
      return dev_id == T::device_id;
    }
    else
      return false;
  }


  template <typename T>
  bool check_has_temp_sensor(const libusb_dev_hdl_uptr& dev_hdl)
  {
    spdlog::debug("verifying device has coolant temperature sensor");
    lusb_msg_t buf;
    buf.fill(0);
    buf[0] = 3; // length
    buf[1] = T::cmd_id++; // command ID
    buf[2] = T::cmd_read_byte; // op-code
    buf[3] = T::cmd_temp_sensor_count; // command data
    lusb_write(dev_hdl, buf);
    // retrieve command response
    buf.fill(0);
    auto rs = lusb_read(dev_hdl, buf);
    if (rs >= 3)
    {
      auto sc = buf[2];
      spdlog::debug("device temp_sensor_count={:d}", sc);
      return sc > 0;
    }
    else
      return false;
  }


  template <typename T>
  std::optional<double> get_coolant_temp(const libusb_dev_hdl_uptr& dev_hdl)
  {
    spdlog::debug("querying coolant temperature");
    lusb_msg_t buf;
    buf.fill(0);
    buf[0] = 7; // length
    buf[1] = T::cmd_id++; // command ID
    buf[2] = T::cmd_write_byte; // op-code
    buf[3] = T::cmd_select_sensor; // command data
    buf[4] = 0; // sensor index
    buf[5] = T::cmd_id++; // command ID
    buf[6] = T::cmd_read_short; // op-code
    buf[7] = T::cmd_sensor_temp; // command data
    lusb_write(dev_hdl, buf);
    // retrieve command response
    buf.fill(0);
    auto rs = lusb_read(dev_hdl, buf);
    if (rs >= 5 && (buf[4] > 0 || buf[5] > 0))
    {
      auto ct =
        static_cast<double>(buf[5]) + static_cast<double>(buf[4]) / 256.;
      spdlog::debug("coolant_temperature={:.2f}C", ct);
      return ct;
    }
    else
      return std::nullopt;
  }


  template <typename T>
  void set_top_fan_max_speed(const libusb_dev_hdl_uptr& dev_hdl)
  {
    spdlog::debug("set_top_fan_max_speed");
    lusb_msg_t buf;
    buf.fill(0);
    buf[0] = 12; // length
    buf[1] = T::cmd_id++; // command ID
    buf[2] = T::cmd_write_byte; // op-code
    buf[3] = T::cmd_select_fan; // command data
    buf[4] = 0; // fan index
    buf[5] = T::cmd_id++; // command ID
    buf[6] = T::cmd_write_byte; // op-code
    buf[7] = T::cmd_fan_mode; // command data
    buf[8] = T::cmd_fan_mode_fixed_pwm; // fan mode
    buf[9] = T::cmd_id++; // command ID
    buf[10] = T::cmd_write_byte; // op-code
    buf[11] = T::cmd_fan_fixed_pwm; // command data
    buf[12] = 0xff; // max pwm
    lusb_write(dev_hdl, buf);
    // dicard command response
    lusb_read(dev_hdl, buf);
  }

} // !namespace cod
