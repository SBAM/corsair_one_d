#ifndef CORSAIR_ONE_D_LIBUSB_WRAPPERS_HPP_
# define CORSAIR_ONE_D_LIBUSB_WRAPPERS_HPP_

# include <memory>
# include <optional>
# include <sstream>
# include <stdexcept>
# include <vector>

# include <libusb.h>

namespace cod
{

  /// @brief Custom deleter for libusb's context
  struct libusb_context_deleter
  {
    constexpr libusb_context_deleter() noexcept = default;
    libusb_context_deleter(const libusb_context_deleter&) noexcept = default;
    void operator()(libusb_context* ctx) const;
  };
  /// shorthand to libusb context unique pointer
  using libusb_context_uptr =
    std::unique_ptr<libusb_context, libusb_context_deleter>;

  /// @return wrapped libusb's context
  libusb_context_uptr make_libusb_context();


  /// @brief Custom deleter for libusb's devices list
  struct libusb_devices_deleter
  {
    constexpr libusb_devices_deleter() noexcept = default;
    libusb_devices_deleter(const libusb_devices_deleter&) noexcept = default;
    void operator()(libusb_device** devs) const;
  };
  /**
   * shorthand to libusb devices unique pointer
   * @note element_type must not be defined as libusb_device[], deleter must
   *       only be invoked on first element
   */
  using libusb_devices_uptr =
    std::unique_ptr<libusb_device*, libusb_devices_deleter>;


  /// @brief Custom delete for libusb's device handle
  struct libusb_dev_hdl_deleter
  {
    constexpr libusb_dev_hdl_deleter() noexcept = default;
    libusb_dev_hdl_deleter(const libusb_dev_hdl_deleter&) noexcept = default;
    void operator()(libusb_device_handle* dev_hdl) const;
  };
  /// shorthand to libusb device handle unique pointer
  using libusb_dev_hdl_uptr =
    std::unique_ptr<libusb_device_handle, libusb_dev_hdl_deleter>;


  class devices_wrapper
  {
  public:
    /// shorthand to device vector
    using devs_vec = std::vector<libusb_device*>;
    /// shorthand to optional device descriptor
    using desc_opt = std::optional<libusb_device_descriptor>;

  public:
    devices_wrapper(const libusb_context_uptr& ctx);
    devices_wrapper(devices_wrapper&&) = default;
    devices_wrapper(const devices_wrapper&) = delete;
    devices_wrapper& operator=(const devices_wrapper&) = delete;

    /// @return lusb device descriptor for vendor/product pair
    desc_opt get_desc(std::uint16_t vnd, std::uint16_t pdt) const;

    /// @return device handle for vendor/product pair
    libusb_dev_hdl_uptr get_dev(std::uint16_t vnd, std::uint16_t pdt) const;

  private:
    libusb_devices_uptr lusb_devs_; ///< stores libusb devices list
    devs_vec devs_; ///< wraps devices list
  };

} // !namespace cod

#endif // !CORSAIR_ONE_D_LIBUSB_WRAPPERS_HPP_
