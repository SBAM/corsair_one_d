#include <cstdlib>
#include <iostream>

#include <boost/program_options.hpp>

#include "daemon.hpp"

namespace po = boost::program_options;

constexpr int EXIT_RESTART = EXIT_FAILURE;
constexpr int EXIT_PREVENT_RESTART = 2;

int main(int argc, const char* argv[])
{
  // manage command line
  bool help_requested = false;
  bool daemon_mode = false;
  po::options_description opt_desc("CorsairOneDaemon options");
  opt_desc.add_options()
    ("help,h", "CorsairOneDaemon usage")
    ("daemon,d", "daemon (background) mode");
  po::variables_map vm;
  try
  {
    po::store(po::parse_command_line(argc, argv, opt_desc), vm);
    daemon_mode = (vm.count("daemon") > 0);
    help_requested = (vm.count("help") > 0);
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return EXIT_PREVENT_RESTART;
  }
  // intialize log_guard
  log_guard lg(daemon_mode);
  // dump help and exit if requested
  if (help_requested)
  {
    lg.quiet_exit(true);
    spdlog::info("{}", opt_desc);
    return EXIT_SUCCESS;
  }
  else
    spdlog::info("daemon_mode={}", daemon_mode);
  try
  {
    cod::sensors_wrapper sw;
    auto lusb_ctx = cod::make_libusb_context();
    cod::devices_wrapper devs(lusb_ctx);
    auto desc = devs.get_desc(cod::defs::vendor_id, cod::defs::product_id);
    if (desc == std::nullopt)
    {
      spdlog::warn("failed to grab description for device with "
                   "vendor_id={:#04x} product_id={:#04x}",
                   cod::defs::vendor_id, cod::defs::product_id);
      return EXIT_PREVENT_RESTART;
    }
    auto dev_hdl = devs.get_dev(cod::defs::vendor_id, cod::defs::product_id);
    if (!dev_hdl)
    {
      spdlog::warn("failed to get USB device handler");
      return EXIT_PREVENT_RESTART;
    }
    // if anything above went wrong, daemon shouldn't be restarted
    if (!cod::check_device_id<cod::defs>(dev_hdl))
    {
      /**
       * @note this mismatch can occur under circumstances not yet understood:
       *        - a request times out
       *        - daemon restarts
       *        - this check fails
       *       let's work this around with a restart exit code
       */
      spdlog::warn("device_id mismatch");
      return EXIT_RESTART;
    }
    if (!cod::check_has_temp_sensor<cod::defs>(dev_hdl))
    {
      spdlog::warn("device has no temperature sensor");
      return EXIT_RESTART;
    }
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
  lg.quiet_exit(true);
  return EXIT_SUCCESS;
}
