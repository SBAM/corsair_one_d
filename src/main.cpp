#include <cstdlib>
#include <iomanip>
#include <iostream>

#include "daemon.hpp"

constexpr int EXIT_RESTART = EXIT_FAILURE;
constexpr int EXIT_PREVENT_RESTART = 2;

int main()
{
  try
  {
    auto lusb_ctx = cod::make_libusb_context();
    cod::devices_wrapper devs(lusb_ctx);
    auto desc = devs.get_desc(cod::defs::vendor_id, cod::defs::product_id);
    if (desc == std::nullopt)
      return EXIT_PREVENT_RESTART;
    auto dev_hdl = devs.get_dev(cod::defs::vendor_id, cod::defs::product_id);
    if (!dev_hdl)
      return EXIT_PREVENT_RESTART;
    if (!cod::check_device_id<cod::defs>(dev_hdl))
      return EXIT_PREVENT_RESTART;
    if (!cod::check_has_temp_sensor<cod::defs>(dev_hdl))
      return EXIT_PREVENT_RESTART;
    // if anything above went wrong, daemon shouldn't be restarted
    cod::daemon codd(dev_hdl, 30, 2s, 10s);
    codd.start();
    cod::sig::wait();
    try
    {
      codd.stop();
    }
    catch (const std::exception& e)
    {
      // stop may convey an exception thrown in daemon thread, restart
      std::cerr << "daemon::stop() exception=[" << e.what() << ']' << std::endl;
      return EXIT_RESTART;
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << "main() exception=[" << e.what() << ']' << std::endl;
    return EXIT_PREVENT_RESTART;
  }
  return EXIT_SUCCESS;
}
