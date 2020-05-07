#ifndef CORSAIR_ONE_D_DAEMON_HPP_
# define CORSAIR_ONE_D_DAEMON_HPP_

# include <chrono>
# include <exception>
# include <thread>

# include <boost/asio.hpp>

# include "libsensors_wrappers.hpp"
# include "link_messaging.hpp"
# include "signal.hpp"

namespace stdc = std::chrono;
using sc = stdc::system_clock;
using namespace std::chrono_literals;

namespace bio = boost::asio;
namespace bs = boost::system;
/// shorthand to work_guard
using bio_work_guard = bio::executor_work_guard<bio::io_context::executor_type>;
/// shorthand to work_guard unique_ptr
using work_guard_uptr = std::unique_ptr<bio_work_guard>;

namespace cod
{

  /// @brief This class manages libusb calls according to various timer delays
  class daemon
  {
  public:
    /**
     * @param sw initialized sensors_wrapper
     * @param dev_hdl device handler
     * @param coretemp_threshold if coretemps' max value goes above this value,
     *                           daemon will trigger a set_top_fan_max_speed
     *                           call
     * @param coolant_temp_threshold if coolant temperature goes above this
     *                               value, daemon will trigger a
     *                               set_top_fan_max_speed call
     * @param low_speed_delay rechecks coolant temperature delay, used when fan
     *                        is running at low speed
     * @param max_speed_delay rechecks coolant temperature delay, used when fan
     *                        is running at max speed
     * @param max_consecutive_failures caps max consecutive failures due to
     *                                 link messaging's calls
     */
    daemon(const sensors_wrapper& sw,
           const libusb_dev_hdl_uptr& dev_hdl,
           double coretemp_threshold,
           double coolant_temp_threshold,
           sc::duration low_speed_delay,
           sc::duration max_speed_delay,
           std::size_t max_consecutive_failures);
    /// @brief invokes stop()
    ~daemon();

    /// @brief immediately checks coolant temperature and starts daemon loop
    void start();
    /// @brief cancels internal timer and stops daemon loop
    void stop();

  private:
    void schedule_timer(sc::duration delay);

  private:
    const sensors_wrapper& sw_; ///< libsensors helper
    const libusb_dev_hdl_uptr& dev_hdl_; ///< device handle
    const double coretemp_threshold_; ///< coretemp threshold
    const double coolant_temp_threshold_; ///< coolant temperature threshold
    const sc::duration low_speed_delay_; ///< fan low speed recheck delay
    const sc::duration max_speed_delay_; ///< fan max speed delay
    const std::size_t max_error_count_; ///< max consecutive errors
    bio::io_context ctx_; ///< asio context
    work_guard_uptr work_; ///< context's work
    bio::steady_timer timer_; ///< delayed tasks timer
    std::thread thread_; ///< reactor thread
    std::size_t error_count_; ///< current consecutive errors
    bool abort_; ///< abort flag, true upon stop request
    std::exception_ptr eptr_; ///< stores execption thrown by daemon
  };

} // !namespace cod

#endif // !CORSAIR_ONE_D_DAEMON_HPP_
