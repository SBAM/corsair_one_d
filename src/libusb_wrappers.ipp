namespace cod
{

  template <typename T>
  void make_lusb_error(T code, const src_loc& loc)
  {
    std::ostringstream err;
    err
      << loc.file_name() << ':'
      << loc.line() << '('
      << loc.function_name() << ") "
      << libusb_error_name(static_cast<std::int32_t>(code)) << ": "
      << libusb_strerror(static_cast<libusb_error>(code));
    throw std::runtime_error(err.str());
  }

} // !namespace cod
