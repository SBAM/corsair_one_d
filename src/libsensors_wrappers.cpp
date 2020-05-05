#include <cstring>
#include <stdexcept>

#include "libsensors_wrappers.hpp"
#include "log.hpp"

namespace cod
{

  sensors_wrapper::sensors_wrapper()
  {
    auto si_res = sensors_init(nullptr);
    if (si_res != 0)
      throw std::runtime_error
        (fmt::format("sensors_init() failure={}", sensors_strerror(si_res)));
    // retrieve all coretemp's INPUT sub-features
    int dc_idx = 0; // detected chip list index
    while (auto dc = sensors_get_detected_chips(nullptr, &dc_idx))
    {
      if (std::strcmp(dc->prefix, "coretemp") != 0)
        continue;
      int f_idx = 0; // features list index
      while (auto feat = sensors_get_features(dc, &f_idx))
      {
        int sf_idx = 0; // subfeatures list index
        while (auto sfeat = sensors_get_all_subfeatures(dc, feat, &sf_idx))
        {
          if (sfeat->type == SENSORS_SUBFEATURE_TEMP_INPUT &&
              sfeat->flags & SENSORS_MODE_R)
            coretemp_sfeats_.push_back({ dc, sfeat });
        }
      }
    }
    if (coretemp_sfeats_.empty())
      throw std::runtime_error("no coretemp returned by libsensors, "
                               "this might be fixed with sensors-detect");
    else
      spdlog::debug("libsensors returned {:d} coretemp sub-features",
                    coretemp_sfeats_.size());
  }


  sensors_wrapper::~sensors_wrapper()
  {
    sensors_cleanup();
  }


  std::optional<double> sensors_wrapper::get_max_coretemp() const
  {
    spdlog::debug("querying max coretemp");
    std::optional<double> res;
    for (auto& [chip, sfeat] : coretemp_sfeats_)
    {
      double tmp {};
      auto gv_res = sensors_get_value(chip, sfeat->number, &tmp);
      if (gv_res == 0)
        res = (res ? std::max(*res, tmp) : tmp);
    }
    if (res)
      spdlog::debug("max_coretemp={:.2f}C", *res);
    return res;
  }

} // !namespace cod
