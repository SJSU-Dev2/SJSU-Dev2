#pragma once

#include <cstddef>
#include <cstdint>

#include "utility/units.hpp"

namespace sjsu
{
class SystemController
{
 private:
  /// Global platform system controller scoped within this class. Most
  /// systems only need a single platform system controller, and thus this
  /// can hold a general/default platform system controller that can be
  /// retrieved via SetPlatformController and GetPlatformController.
  static inline SystemController * platform_system_controller = nullptr;

 public:
  /// Set the controller for the platform
  ///
  /// @param system_controller - a pointer to the current platform's
  ///        system controller.
  static void SetPlatformController(SystemController * system_controller)
  {
    platform_system_controller = system_controller;
  }
  /// Retrieve a reference of the platforms system controller
  static sjsu::SystemController & GetPlatformController()
  {
    return *platform_system_controller;
  }

  // ==============================
  // Interface Defintions
  // ==============================

  class PeripheralID
  {
   public:
    uint8_t device_id = -1;
  };

  template <size_t kDeviceId>
  class AddPeripheralID : public PeripheralID
  {
   public:
    constexpr AddPeripheralID()
    {
      device_id = kDeviceId;
    }
  };

  // ==============================
  // Interface Methods
  // ==============================

  /// Control PLLs and dividers to set the system clock rate
  ///
  /// @returns the difference between the frequency supplied and the frequency
  ///          achieved.
  virtual void SetSystemClockFrequency(
      units::frequency::megahertz_t frequency_in_mhz) const = 0;
  /// Set the peripheral/bus clock frequency divider
  virtual void SetPeripheralClockDivider(const PeripheralID &,
                                         uint8_t peripheral_divider) const = 0;
  /// @return peripheral clock divider
  virtual uint32_t GetPeripheralClockDivider(const PeripheralID &) const = 0;
  /// @return system clock frequency
  virtual units::frequency::hertz_t GetSystemFrequency() const = 0;
  /// Checks hardware and determines if the peripheral is powered up
  virtual bool IsPeripheralPoweredUp(
      const PeripheralID & peripheral_select) const = 0;
  /// Powers up the supplied peripheral
  virtual void PowerUpPeripheral(
      const PeripheralID & peripheral_select) const = 0;
  /// Powers down the supplied peripheral
  virtual void PowerDownPeripheral(
      const PeripheralID & peripheral_select) const = 0;

  // ==============================
  // Utility Methods
  // ==============================

  /// @returns current bus/peripheral operating frequency
  units::frequency::hertz_t GetPeripheralFrequency(
      const PeripheralID & peripheral_select) const
  {
    uint32_t peripheral_clock_divider =
        GetPeripheralClockDivider(peripheral_select);
    // return 0 if peripheral_clock_divider == 0
    units::frequency::hertz_t result = 0_Hz;
    if (peripheral_clock_divider != 0)
    {
      result = GetSystemFrequency() / peripheral_clock_divider;
    }
    return result;
  }
};
}  // namespace sjsu
