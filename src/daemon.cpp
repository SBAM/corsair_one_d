#include "daemon.hpp"

namespace cod
{

  daemon::daemon(const sensors_wrapper& sw,
                 const libusb_dev_hdl_uptr& dev_hdl,
                 double coretemp_threshold,
                 double coolant_temp_threshold,
                 sc::duration low_speed_delay,
                 sc::duration max_speed_delay) :
    sw_(sw),
    dev_hdl_(dev_hdl),
    coretemp_threshold_(coretemp_threshold),
    coolant_temp_threshold_(coolant_temp_threshold),
    low_speed_delay_(low_speed_delay),
    max_speed_delay_(max_speed_delay),
    work_(std::make_unique<bio_work_guard>(bio::make_work_guard(ctx_))),
    timer_(ctx_),
    abort_(false),
    eptr_(nullptr)
  {
  }


  daemon::~daemon()
  {
    work_.reset();
    stop();
    if (thread_.joinable())
      thread_.join();
  }


  void daemon::start()
  {
    auto start_cb = [this]()
      {
        schedule_timer(0s);
      };
    bio::post(ctx_, std::move(start_cb));
    auto thread_cb = [this]()
      {
        try
        {
          ctx_.run();
        }
        catch (...)
        {
          eptr_ = std::current_exception();
          sig::leave(); // unblock main thread
        }
      };
    thread_ = std::thread(std::move(thread_cb));
  }


  void daemon::stop()
  {
    if (eptr_ && !std::current_exception())
    {
      // daemon thread stored an exception_ptr and no unwinding is in progress
      auto tmp = std::move(eptr_); // resets eptr_ to nullptr
      std::rethrow_exception(tmp); // rethrow daemon exception_ptr
    }
    auto stop_cb = [this]()
      {
        if (!abort_)
        {
          abort_ = true;
          timer_.cancel();
        }
      };
    bio::post(ctx_, std::move(stop_cb));
  }


  void daemon::schedule_timer(sc::duration delay)
  {
    if (abort_)
      return;
    timer_.expires_after(delay);
    auto cb = [this](auto ec)
      {
        if (abort_ || ec == bio::error::operation_aborted)
          return;
        // check coretemp first, most likely will exceed threshold first
        std::optional<double> core_temp;
        if (core_temp = sw_.get_max_coretemp();
            core_temp.has_value())
        {
          if (*core_temp > coretemp_threshold_)
          {
            spdlog::info("threshold exceeded, "
                         "core={:.2f}C > (max={:.2f}C) "
                         "==> set max top fan speed",
                         *core_temp, coretemp_threshold_);
            set_top_fan_max_speed<defs>(dev_hdl_);
            schedule_timer(max_speed_delay_);
            return;
          }
        }
        else
          spdlog::warn("failed to actualize coretemp");
        // then check coolant temperature
        std::optional<double> cool_temp;
        try
        {
          if (cool_temp = get_coolant_temp<defs>(dev_hdl_);
              cool_temp.has_value())
          {
            if (*cool_temp > coolant_temp_threshold_)
            {
              spdlog::info("threshold exceeded, "
                           "coolant={:.2f}C (max={:.2f}C) "
                           "==> set max top fan speed",
                           *cool_temp, coolant_temp_threshold_);
              set_top_fan_max_speed<defs>(dev_hdl_);
              schedule_timer(max_speed_delay_);
              return;
            }
          }
          else
            spdlog::warn("failed to actualize coolant temperature");
        }
        catch (const std::exception& e)
        {
          spdlog::warn("failed to grab coolant temperature [{}]", e.what());
        }
        // both temperatures were retrieved and below thresholds
        if (core_temp.has_value() && cool_temp.has_value())
          schedule_timer(low_speed_delay_);
        else
          schedule_timer(100ms); // fast reschedule
      };
    timer_.async_wait(std::move(cb));
  }

} // !namespace cod
