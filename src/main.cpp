#include <iomanip>
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
    std::cout
      << "found description for vnd=" << cod::defs::vendor_id
      << " pdt=" << cod::defs::product_id << std::endl;
    auto dev_hdl = devs.get_dev(cod::defs::vendor_id, cod::defs::product_id);
    if (!dev_hdl)
      return 43;
    std::cout << "got device handle" << std::endl;
    if (!cod::check_device_id<cod::defs>(dev_hdl))
      return 44;
    std::cout
      << "matched device_id=" << static_cast<int>(cod::defs::device_id)
      << std::endl;
    if (!cod::check_has_temp_sensor<cod::defs>(dev_hdl))
      return 45;
    std::cout << "has temperature sensor" << std::endl;
    auto temp = cod::get_coolant_temp<cod::defs>(dev_hdl);
    if (temp == std::nullopt)
      return 46;
    std::cout
      << "temp=" << std::fixed << std::setprecision(1) << *temp << std::endl;
    cod::set_top_fan_max_speed<cod::defs>(dev_hdl);
    std::cout << "set top fan to max speed" << std::endl;
  }
  catch (const std::exception& e)
  {
    std::cerr << "caught exception=[" << e.what() << ']' << std::endl;
    return 1;
  }
  return 0;
}
