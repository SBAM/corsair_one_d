#include "libusb_wrappers.hpp"

namespace cod
{

  void libusb_context_deleter::operator()(libusb_context* ctx) const
  {
    libusb_exit(ctx);
  }

  libusb_context_uptr make_libusb_context()
  {
     libusb_context* ctx = nullptr;
     auto ret = libusb_init(&ctx);
     if (ret != LIBUSB_SUCCESS)
       make_lusb_error(ret);
     return libusb_context_uptr(ctx);
  }


  void libusb_devices_deleter::operator()(libusb_device** devs) const
  {
    libusb_free_device_list(devs, 1); // decrements reference counter
  }


  void libusb_dev_hdl_deleter::operator()(libusb_device_handle* dev_hdl) const
  {
    libusb_release_interface(dev_hdl, 0);
    libusb_close(dev_hdl);
  }


  void lusb_write(const libusb_dev_hdl_uptr& dev_hdl, lusb_msg_t& dat)
  {
    auto ret = libusb_control_transfer
      (dev_hdl.get(), // device handle
       LIBUSB_ENDPOINT_OUT | // request type
       LIBUSB_REQUEST_TYPE_CLASS |
       LIBUSB_RECIPIENT_INTERFACE,
       LIBUSB_REQUEST_SET_CONFIGURATION, // request
       0x0200, // value HID_REPORT_TYPE_OUTPUT
       0x0000, // interface index
       dat.data(), // payload
       static_cast<std::uint16_t>(dat.size()), // payload length
       1000); // 1000ms timeout
    if (ret < 0)
      make_lusb_error(ret);
  }

  std::int32_t lusb_read(const libusb_dev_hdl_uptr& dev_hdl, lusb_msg_t& dat)
  {
    std::int32_t transferred = {};
    auto ret = libusb_interrupt_transfer
      (dev_hdl.get(), // device handle
       LIBUSB_ENDPOINT_IN | // endpoint
       LIBUSB_RECIPIENT_INTERFACE,
       dat.data(), // payload
       static_cast<std::uint16_t>(dat.size()), // payload length,
       &transferred, // bytes transferred,
       1000); // 1000ms timeout
    if (ret < 0)
      make_lusb_error(ret);
    return transferred;
  }


  devices_wrapper::devices_wrapper(const libusb_context_uptr& ctx)
  {
    libusb_device** devs = nullptr;
    auto dev_count = libusb_get_device_list(ctx.get(), &devs);
    if (dev_count < 0)
      make_lusb_error(dev_count);
    else
    {
      lusb_devs_.reset(devs);
      devs_.insert(devs_.begin(), devs, devs + dev_count);
    }
  }

  auto devices_wrapper::get_desc(std::uint16_t vnd, std::uint16_t pdt) const
    -> desc_opt
  {
    for (auto* dev : devs_)
    {
      libusb_device_descriptor desc;
      auto ret = libusb_get_device_descriptor(dev, &desc);
      if (ret != LIBUSB_SUCCESS)
        make_lusb_error(ret);
      if (desc.idVendor == vnd && desc.idProduct == pdt)
        return desc;
    }
    return std::nullopt;
  }

  libusb_dev_hdl_uptr
  devices_wrapper::get_dev(std::uint16_t vnd, std::uint16_t pdt) const
  {
    for (auto* dev : devs_)
    {
      libusb_device_descriptor desc;
      auto ret = libusb_get_device_descriptor(dev, &desc);
      if (ret != LIBUSB_SUCCESS)
        make_lusb_error(ret);
      if (desc.idVendor == vnd && desc.idProduct == pdt)
      {
        libusb_device_handle* hdl = nullptr;
        ret = libusb_open(dev, &hdl);
        if (ret != LIBUSB_SUCCESS)
          make_lusb_error(ret);
        libusb_dev_hdl_uptr res(hdl);
        ret = libusb_set_auto_detach_kernel_driver(hdl, 1);
        if (ret != LIBUSB_SUCCESS)
          make_lusb_error(ret);
        ret = libusb_claim_interface(hdl, 0);
        if (ret != LIBUSB_SUCCESS)
          make_lusb_error(ret);
        return res;
      }
    }
    return nullptr;
  }

} // !namespace cod
