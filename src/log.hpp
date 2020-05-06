#ifndef CORSAIR_ONE_D_LOG_HPP_
# define CORSAIR_ONE_D_LOG_HPP_

# include <spdlog/spdlog.h>
# include <spdlog/fmt/bin_to_hex.h>
# include <spdlog/fmt/ostr.h>
# include <spdlog/sinks/sink.h>

/// @brief Manages logs
class log_guard
{
public:
  /// @brief Initializes default logger
  log_guard(bool daemon_mode);
  /// @brief Dumps backtrace in all sinks
  ~log_guard();
  log_guard(const log_guard&) = delete;
  log_guard& operator=(const log_guard&) = delete;

  /// @param qe if true, log_guard will be destroyed silently
  void quiet_exit(bool qe);

private:
  bool daemon_mode_; ///< defines how sinks should be configured
  bool quiet_exit_; ///< defines behavior upon destruction
};

#endif // !CORSAIR_ONE_D_LOG_HPP_
