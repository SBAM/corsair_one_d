#ifndef CORSAIR_ONE_D_LINK_MESSAGING_HPP_
# define CORSAIR_ONE_D_LINK_MESSAGING_HPP_

# include "libusb_wrappers.hpp"

namespace cod
{

  namespace details
  {

    /// @brief Regroups devices details for CS-9000015-eu
    struct defs_cs_9000015_eu
    {
      static constexpr std::uint16_t vendor_id = 0x1b1c;
      static constexpr std::uint16_t product_id = 0x0c04;
      static constexpr std::uint8_t device_id = 0x50;
      static constexpr std::uint8_t cmd_id_init = 0x81;
      static constexpr std::uint8_t cmd_read_byte = 0x07;
      static constexpr std::uint8_t cmd_dev_id = 0x00;

      static std::uint8_t cmd_id;
    };

  } // !namespace details

  /// Default device definitions, leaves room for later improvements
  using defs = details::defs_cs_9000015_eu;


  template <typename T>
  bool check_device_id(const libusb_dev_hdl_uptr& dev_hdl, std::uint8_t id);

} // !namespace cod

# include "link_messaging.ipp"

#endif // !CORSAIR_ONE_D_LINK_MESSAGING_HPP_
