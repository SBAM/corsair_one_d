#include "libusb_wrappers.hpp"

namespace
{

  template <typename T>
  void make_lusb_error(T code)
  {
    std::ostringstream err;
    err
      << libusb_error_name(static_cast<std::int32_t>(code)) << ": "
      << libusb_strerror(static_cast<libusb_error>(code));
    throw std::runtime_error(err.str());
  }

} // !anonymous namespace


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
