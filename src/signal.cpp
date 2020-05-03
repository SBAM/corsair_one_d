#include <string>
#include <unordered_map>

#include "log.hpp"
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


  void sig::handler(int sig_id)
  {
    static const std::unordered_map<int, std::string> map_ =
      {
        { SIGTERM, "SIGTERM" },
        { SIGSEGV, "SIGSEGV" },
        { SIGINT,  "SIGINT" },
        { SIGILL,  "SIGILL" },
        { SIGABRT, "SIGABRT" },
        { SIGFPE,  "SIGFPE" }
      };
    auto it = map_.find(sig_id);
    if (it != map_.end())
      spdlog::debug("signal {}({}) was raised", it->second, sig_id);
    else
      spdlog::warn("unknown signal ({}) was raised", sig_id);
    leave();
  }

} // !namespace cod
