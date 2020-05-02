/// @todo remove me
#include <iostream>

#include "signal.hpp"

namespace cod
{

  std::mutex sig::mutex_;
  std::condition_variable sig::cond_;
  bool sig::cond_check_ = false;

  void sig::wait()
  {
    // install signal handlers
    std::signal(SIGTERM, sig::handler);
    std::signal(SIGSEGV, SIG_DFL);
    std::signal(SIGINT, sig::handler);
    std::signal(SIGILL, SIG_DFL);
    std::signal(SIGABRT, SIG_DFL);
    std::signal(SIGFPE, SIG_DFL);
    auto check_cond = []()
      {
        return cond_check_;
      };
    std::unique_lock ul(mutex_);
    cond_.wait(ul, check_cond);
  }


  void sig::leave()
  {
    {
      std::scoped_lock sl(mutex_);
      cond_check_ = true;
    }
    cond_.notify_one();
  }


  void sig::handler(int)
  {
    /// @todo remove me
    std::cout << "signal caught" << std::endl;
    leave();
  }

} // !namespace cod
