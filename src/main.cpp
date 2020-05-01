#include <iostream>

#include "libusb_wrappers.hpp"

int main()
{
  try
  {
    auto lusb_ctx = cod::make_libusb_context();
    cod::devices_wrapper devs(lusb_ctx);
    auto desc = devs.get_desc(6940, 3076);
    if (desc == std::nullopt)
      return 42;
    auto dev_hdl = devs.get_dev(6940, 3076);
    if (!dev_hdl)
      return 43;
  }
  catch (const std::exception& e)
  {
    std::cerr << "caught exception=[" << e.what() << ']' << std::endl;
    return 1;
  }
  return 0;
}
