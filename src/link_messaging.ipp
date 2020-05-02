namespace cod
{

  template <typename T>
  bool check_device_id(const libusb_dev_hdl_uptr& dev_hdl)
  {
    lusb_msg_t buf;
    buf.fill(0);
    buf[0] = 3; // length
    buf[1] = T::cmd_id++; // command ID
    buf[2] = T::cmd_read_byte; // op-code
    buf[3] = T::cmd_dev_id; // command data
    lusb_write(dev_hdl, buf);
    // retrieve command response
    buf.fill(0);
    auto rs = lusb_read(dev_hdl, buf);
    if (rs >= 3)
      return buf[2] == T::device_id;
    else
      return false;
  }

} // !namespace cod
