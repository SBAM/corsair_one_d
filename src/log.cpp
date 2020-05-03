#include "log.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/syslog_sink.h>

void setup_logger()
{
  auto cout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  cout_sink->set_level(spdlog::level::trace);
  cout_sink->set_pattern("%Y-%m-%d %T.%e %^%l %v");
  auto sys_sink =
    std::make_shared<spdlog::sinks::syslog_sink_mt>("CorsairOneDaemon",
                                                    0, // no option
                                                    LOG_DAEMON, // facility
                                                    true); // enable formatting
  sys_sink->set_level(spdlog::level::debug);
  sys_sink->set_pattern("%l %v");
  std::vector<spdlog::sink_ptr> sinks { cout_sink, sys_sink};
  auto logger = std::make_shared<spdlog::logger>("multi_sink",
                                                 sinks.begin(),
                                                 sinks.end());
  logger->set_level(spdlog::level::trace);
  spdlog::set_default_logger(logger);
}
