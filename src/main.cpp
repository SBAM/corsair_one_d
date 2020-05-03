#include <cstdlib>

#include "daemon.hpp"
#include "log.hpp"

constexpr int EXIT_RESTART = EXIT_FAILURE;
constexpr int EXIT_PREVENT_RESTART = 2;

int main()
{
  setup_logger();
  try
  {
    cod::sensors_wrapper sw;
    auto lusb_ctx = cod::make_libusb_context();
    cod::devices_wrapper devs(lusb_ctx);
    auto desc = devs.get_desc(cod::defs::vendor_id, cod::defs::product_id);
    if (desc == std::nullopt)
    {
      spdlog::warn("main() failed to grab description for device with "
                   "vendor_id={0:#x} product_id={0:#x}",
                   cod::defs::vendor_id, cod::defs::product_id);
      return EXIT_PREVENT_RESTART;
    }
    auto dev_hdl = devs.get_dev(cod::defs::vendor_id, cod::defs::product_id);
    if (!dev_hdl)
    {
      spdlog::warn("main() failed to get USB device handler");
      return EXIT_PREVENT_RESTART;
    }
    if (!cod::check_device_id<cod::defs>(dev_hdl))
    {
      spdlog::warn("main() device handle doesn't match device_id={0:#x}",
                   cod::defs::device_id);
      return EXIT_PREVENT_RESTART;
    }
    if (!cod::check_has_temp_sensor<cod::defs>(dev_hdl))
    {
      spdlog::warn("main() device has no temperature sensor");
      return EXIT_PREVENT_RESTART;
    }
    // if anything above went wrong, daemon shouldn't be restarted
    cod::daemon codd(sw, dev_hdl, 60, 40, 2s, 10s);
    codd.start();
    cod::sig::wait();
    try
    {
      codd.stop();
    }
    catch (const std::exception& e)
    {
      // stop may convey an exception thrown in daemon thread, restart
      spdlog::error("daemon::stop() exception=[{}]", e.what());
      return EXIT_RESTART;
    }
  }
  catch (const std::exception& e)
  {
    spdlog::error("main() exception=[{}]", e.what());
    return EXIT_PREVENT_RESTART;
  }
  return EXIT_SUCCESS;
}
