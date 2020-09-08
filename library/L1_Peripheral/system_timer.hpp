// SystemTimer abstracts the process of changing enabling and setting
// up the SystemTimer.
#pragma once

#include <cstdint>
#include <functional>

#include "L1_Peripheral/interrupt.hpp"
#include "utility/status.hpp"
#include "utility/units.hpp"

namespace sjsu
{
/// A system timer is a general timer used primarily for generating an interrupt
/// at a fixed period, like 1ms or 10ms. Such interrupts are generally used to
/// give control of the processor back to an operating.
/// @ingroup l1_peripheral
class SystemTimer
{
 public:
  // ===========================================================================
  // Interface Methods
  // ===========================================================================
  /// Initialize system timer hardware.
  virtual Returns<void> Initialize() const = 0;

  /// Set the function to be called when the System Timer interrupt fires.
  ///
  /// @param callback - function to be called on system timer event.
  virtual Returns<void> SetCallback(InterruptCallback callback) const = 0;

  /// Set frequency of the timer.
  ///
  /// @param frequency - How many times per second should the system timer
  ///         interrupt be called.
  /// @return the difference between the frequency that was achieved vs the
  ///         input frequency.
  virtual Returns<int32_t> SetTickFrequency(
      units::frequency::hertz_t frequency) const = 0;

  /// Start the system timer. Should be done after SetInterrupt and
  /// SetTickFrequency have been called.
  virtual Returns<void> StartTimer() const = 0;
};
}  // namespace sjsu
