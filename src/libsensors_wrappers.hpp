#ifndef CORSAIR_ONE_D_LIBSENSORS_WRAPPERS_HPP_
# define CORSAIR_ONE_D_LIBSENSORS_WRAPPERS_HPP_

# include <optional>
# include <vector>

# include <sensors/error.h>
# include <sensors/sensors.h>

namespace cod
{

  /**
   * @brief This class searches for coretemp inputs returned by libsensors
   *        and provides a max-coretemp getter.
   */
  class sensors_wrapper
  {
  public:
    /// shorthand to chip/subfeature pair
    using chip_sfeat_p = std::pair<const sensors_chip_name*,
                                   const sensors_subfeature*>;
    /// shorthant to chip/subfeature vector
    using chip_sfeat_vec = std::vector<chip_sfeat_p>;

  public:
    /// @brief Initializes libsensors and extracts all coretemp's sub-features
    sensors_wrapper();
    /// @brief Cleans up libsensors facilities
    ~sensors_wrapper();

    /// @return max temperature across all coretemps
    std::optional<double> get_max_coretemp() const;

  private:
    chip_sfeat_vec coretemp_sfeats_; ///< stores coretemp chip/sub-features
  };

} // !namespace cod

#endif // !CORSAIR_ONE_D_LIBSENSORS_WRAPPERS_HPP_
