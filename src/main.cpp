#include <iostream>

#include "link_messaging.hpp"

int main()
{
  try
  {
    auto lusb_ctx = cod::make_libusb_context();
    cod::devices_wrapper devs(lusb_ctx);
    auto desc = devs.get_desc(cod::defs::vendor_id, cod::defs::product_id);
    if (desc == std::nullopt)
      return 42;
    auto dev_hdl = devs.get_dev(cod::defs::vendor_id, cod::defs::product_id);
    if (!dev_hdl)
      return 43;
    if (!cod::check_device_id<cod::defs>(dev_hdl))
      return 44;
  }
  catch (const std::exception& e)
  {
    std::cerr << "caught exception=[" << e.what() << ']' << std::endl;
    return 1;
  }
  return 0;
}
