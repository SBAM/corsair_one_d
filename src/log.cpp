#include "log.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/syslog_sink.h>

log_guard::log_guard(bool daemon_mode) :
  daemon_mode_(daemon_mode),
  quiet_exit_(false)
{
  std::vector<spdlog::sink_ptr> sinks;
  if (!daemon_mode)
  {
    auto cout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    cout_sink->set_level(spdlog::level::trace);
    cout_sink->set_pattern("%^%Y-%m-%d %T.%e %l %v%$");
    cout_sink->set_color(spdlog::level::trace, cout_sink->dark);
    cout_sink->set_color(spdlog::level::info, cout_sink->white);
    sinks.push_back(std::move(cout_sink));
  }
  {
    auto sys_sink = std::make_shared<spdlog::sinks::syslog_sink_mt>
      ("CorsairOneDaemon",
       0, // no option
       LOG_DAEMON, // facility
       true); // enable formatting
    sys_sink->set_level(spdlog::level::off);
    sys_sink->set_pattern("%l %v");
    sinks.push_back(std::move(sys_sink));
  }
  auto logger = std::make_shared<spdlog::logger>("multi_sink",
                                                 sinks.begin(),
                                                 sinks.end());
  logger->set_level(spdlog::level::trace);
  spdlog::set_default_logger(logger);
  spdlog::enable_backtrace(16);
}


log_guard::~log_guard()
{
  if (!quiet_exit_)
  {
    for (auto& sink : spdlog::default_logger()->sinks())
      sink->set_level(spdlog::level::trace);
    spdlog::dump_backtrace();
  }
}


void log_guard::quiet_exit(bool qe)
{
  quiet_exit_ = qe;
}
