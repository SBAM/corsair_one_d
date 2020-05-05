namespace cod
{

  template <typename T>
  std::string make_lusb_error_str(T code, const src_loc& loc)
  {
    return fmt::format
      ("{}:{}({}) {}: {}",
       std::filesystem::path(loc.file_name()).filename().string(),
       loc.line(),
       loc.function_name(),
       libusb_error_name(static_cast<std::int32_t>(code)),
       libusb_strerror(static_cast<libusb_error>(code)));
  }

  template <typename T>
  void make_lusb_error(T code, const src_loc& loc)
  {
    throw std::runtime_error(make_lusb_error_str(code, loc));
  }

} // !namespace cod
