#ifndef CORSAIR_ONE_D_SIGNAL_HPP_
# define CORSAIR_ONE_D_SIGNAL_HPP_

# include <condition_variable>
# include <csignal>
# include <mutex>

namespace cod
{

  /// @brief This class handles signals to ensure graceful daemon termination
  class sig
  {
  public:
    /// @brief Blocks calling thread and waits for a leave call or signal
    static void wait();
    /// @brief Un-blocks thread that invoked wait
    static void leave();

  private:
    /**
     * @brief Customized signal handler.
     * @param sig_id Signal indentifier
     */
    static void handler(int sig_id);

  private:
    static std::mutex mutex_; ///< condition variable mutex
    static std::condition_variable cond_; ///< internal condition variable
    static bool cond_check_; ///< prevents spurious wakeup issues
  };

} // !namespace cod

#endif // !CORSAIR_ONE_D_SIGNAL_HPP_
