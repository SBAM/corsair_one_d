/// @todo remove me
#include <iomanip>
#include <iostream>

#include "daemon.hpp"

namespace cod
{

  daemon::daemon(const libusb_dev_hdl_uptr& dev_hdl,
                 float temp_threshold,
                 sc::duration low_speed_delay,
                 sc::duration max_speed_delay) :
    dev_hdl_(dev_hdl),
    temp_threshold_(temp_threshold),
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
        auto ct = get_coolant_temp<defs>(dev_hdl_);
        if (ct)
        {
          if (*ct > temp_threshold_)
          {
            /// @todo remove me
            std::cout
              << "curr_temp=" << std::fixed << std::setprecision(1) << *ct
              << " > threshold=" << std::fixed << std::setprecision(1)
              << temp_threshold_ << std::endl;
            set_top_fan_max_speed<defs>(dev_hdl_);
            schedule_timer(max_speed_delay_);
          }
          else
          {
            /// @todo remove me
            std::cout
              << "curr_temp=" << std::fixed << std::setprecision(1) << *ct
              << " < threshold=" << std::fixed << std::setprecision(1)
              << temp_threshold_ << std::endl;
            schedule_timer(low_speed_delay_);
          }
        }
        else
        {
          /// @todo remove me
          std::cout << "failed to grab coolant temperature" << std::endl;
          schedule_timer(low_speed_delay_);
        }
      };
    timer_.async_wait(std::move(cb));
  }

} // !namespace cod
