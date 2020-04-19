//  Usage:
//      // Two different methods of creating the Pwm object
//      Pwm p2_0 = Pwm::CreatePwm<1>();
//      Pwm p2_0(1);
//      p2_0.Initialize(500);
//      p2_0.SetDutyCycle(0.75);
//      p2_0.SetFrequency(1'000);
#pragma once

#include <cstdint>
#include <cstdio>

#include "L1_Peripheral/pwm.hpp"

#include "L0_Platform/lpc40xx/LPC40xx.h"
#include "L1_Peripheral/lpc40xx/pin.hpp"
#include "L1_Peripheral/lpc40xx/system_controller.hpp"
#include "utility/bit.hpp"
#include "utility/log.hpp"
#include "utility/status.hpp"

namespace sjsu
{
namespace lpc40xx
{
/// An implementation of the sjsu::Pwm peripheral for the LPC40xx family of
/// microcontrollers.
///
/// @warning - Changing the frequency of 1 PWM channel, changes the frequency
/// for all other PWM channels.
class Pwm final : public sjsu::Pwm
{
 public:
  /// Output Control bit masks.
  struct OutputControl  // NOLINT
  {
    /// Enables/disables double edge PWM for each channel. Each bit in this
    /// field corrissponds to a channel.
    constexpr static bit::Mask kEnableDoubleEdge =
        bit::CreateMaskFromRange(2, 6);
    /// Enables/disables output of PWM channels. Each bit from the start
    /// corrissponds to a PWM channel.
    constexpr static bit::Mask kEnableOutput = bit::CreateMaskFromRange(8, 14);
  };

  /// Match register control bit masks.
  struct MatchControl  // NOLINT
  {
    /// If set to a 1, tells the PWM hardare to reset the PWM total count
    /// register to be reset to 0 when it is equal to the match register 0.
    constexpr static bit::Mask kPwm0Reset = bit::CreateMaskFromRange(1);
  };

  /// Register controls PWM peripheral wide timer control.
  struct Timer  // NOLINT
  {
    /// When set to a 1, enables the TC (total count) register and begins
    /// counting.
    constexpr static bit::Mask kCounterEnable = bit::CreateMaskFromRange(0);
    /// When set to a 1, will reset the total count register.
    constexpr static bit::Mask kCounterReset = bit::CreateMaskFromRange(1);
    /// Enables PWM mode. Without setting the match registers cannot operate
    /// with the timer.
    constexpr static bit::Mask kPwmEnable = bit::CreateMaskFromRange(3);
    /// Allows PWM0 peripheral to control the PWM1 peripheral. This is generally
    /// set to 0, to allow both of them to work independently.
    constexpr static bit::Mask kMasterDisable = bit::CreateMaskFromRange(4);
  };

  /// PWM channel count control register
  struct CountControl  // NOLINT
  {
    /// Controls the counting mode of the PWM peripheral. When set to 0, counts
    /// using the internal prescale counter which is driven by the peripheral
    /// clock. Other modes involve use of an external clock source.
    constexpr static bit::Mask kMode = bit::CreateMaskFromRange(0, 1);
    /// Controls the count input source. For this driver, this should be kept to
    /// zero, since we want to utilize the internal peripheral clock source.
    constexpr static bit::Mask kCountInput = bit::CreateMaskFromRange(2, 3);
  };

  /// Defines a LPC PWM peripheral definition that contains all of the
  /// information needed to use it.
  struct Peripheral_t
  {
    /// Holds the address to the LPC PWM peripheral.
    LPC_PWM_TypeDef * registers;
    /// The power on id for the above LPC PWM peripheral.
    sjsu::SystemController::PeripheralID id;
  };

  /// Defines all information necessary to control a specific PWM channel.
  struct Channel_t
  {
    /// Reference to the Peripheral_t that this channel belongs to.
    const Peripheral_t & peripheral;
    /// Reference to the pwm pin.
    const sjsu::Pin & pin;
    /// Reference to the PWM channel number.
    uint8_t channel : 3;
    /// Contains the pin function id, used to select PWM output for the pin.
    uint8_t pin_function_code : 3;
  };

  /// Structure used as a namespace for predefined Channel definitions
  struct Channel  // NOLINT
  {
   private:
    inline static const sjsu::lpc40xx::Pin kPwmPin0 =
        sjsu::lpc40xx::Pin::CreatePin<2, 0>();
    inline static const sjsu::lpc40xx::Pin kPwmPin1 =
        sjsu::lpc40xx::Pin::CreatePin<2, 1>();
    inline static const sjsu::lpc40xx::Pin kPwmPin2 =
        sjsu::lpc40xx::Pin::CreatePin<2, 2>();
    inline static const sjsu::lpc40xx::Pin kPwmPin3 =
        sjsu::lpc40xx::Pin::CreatePin<2, 3>();
    inline static const sjsu::lpc40xx::Pin kPwmPin4 =
        sjsu::lpc40xx::Pin::CreatePin<2, 4>();
    inline static const sjsu::lpc40xx::Pin kPwmPin5 =
        sjsu::lpc40xx::Pin::CreatePin<2, 5>();

   public:
    /// Definition of the PWM 0 peripheral.
    inline static const Peripheral_t kPwm0Peripheral = {
      .registers = LPC_PWM0,
      .id        = sjsu::lpc40xx::SystemController::Peripherals::kPwm1,
    };
    /// Definition of the PWM 1 peripheral.
    inline static const Peripheral_t kPwm1Peripheral = {
      .registers = LPC_PWM1,
      .id        = sjsu::lpc40xx::SystemController::Peripherals::kPwm1,
    };
    /// Definition for channel 0 of PWM peripheral 1.
    inline static const Channel_t kPwm0 = {
      .peripheral        = kPwm1Peripheral,
      .pin               = kPwmPin0,
      .channel           = 1,
      .pin_function_code = 0b001,
    };
    /// Definition for channel 1 of PWM peripheral 1.
    inline static const Channel_t kPwm1 = {
      .peripheral        = kPwm1Peripheral,
      .pin               = kPwmPin1,
      .channel           = 2,
      .pin_function_code = 0b001,
    };
    /// Definition for channel 2 of PWM peripheral 1.
    inline static const Channel_t kPwm2 = {
      .peripheral        = kPwm1Peripheral,
      .pin               = kPwmPin2,
      .channel           = 3,
      .pin_function_code = 0b001,
    };
    /// Definition for channel 3 of PWM peripheral 1.
    inline static const Channel_t kPwm3 = {
      .peripheral        = kPwm1Peripheral,
      .pin               = kPwmPin3,
      .channel           = 4,
      .pin_function_code = 0b001,
    };
    /// Definition for channel 4 of PWM peripheral 1.
    inline static const Channel_t kPwm4 = {
      .peripheral        = kPwm1Peripheral,
      .pin               = kPwmPin4,
      .channel           = 5,
      .pin_function_code = 0b001,
    };
    /// Definition for channel 5 of PWM peripheral 1.
    inline static const Channel_t kPwm5 = {
      .peripheral        = kPwm1Peripheral,
      .pin               = kPwmPin5,
      .channel           = 6,
      .pin_function_code = 0b001,
    };
  };

  /// Constructor for a LPC40xx PWM channel.
  ///
  /// @param channel - Reference to a const channel description for this
  ///        instance of the PWM driver.
  explicit constexpr Pwm(const Channel_t & channel) : channel_(channel) {}
  Status Initialize(units::frequency::hertz_t frequency_hz) const override
  {
    SJ2_ASSERT_FATAL(1 <= channel_.channel && channel_.channel <= 6,
                     "Channel must be between 1 and 6 on LPC40xx platforms.");

    auto & system = sjsu::SystemController::GetPlatformController();

    system.PowerUpPeripheral(channel_.peripheral.id);

    auto * pwm = channel_.peripheral.registers;
    // Set prescalar to 1 so the input frequency to the PWM peripheral is equal
    // to the peripheral clock frequency.
    //
    // PR = Prescale register = defines the maximum value for the prescaler
    pwm->PR = 0;

    // PC = Prescale counter. Set to 0 to cause the timer counter to increment
    // after each peripheral clock tick.
    pwm->PC = 0;

    // Mode 0x0 means increment time counter (TC) after the peripheral clock
    // has cycled the amount inside of the prescale.
    pwm->CTCR = bit::Insert(pwm->CTCR, 0, CountControl::kMode);

    // 0x0 for this register says to use the TC for input counts.
    pwm->CTCR = bit::Insert(pwm->CTCR, 0, CountControl::kCountInput);

    // Match register 0 is used to generate the desired frequency. If the time
    // counter TC is equal to MR0
    const units::frequency::hertz_t kPeripheralFrequency =
        system.GetClockRate(channel_.peripheral.id);

    pwm->MR0 = (kPeripheralFrequency / frequency_hz).to<uint32_t>();

    // Sets match register 0 to reset when TC and Match 0 match each other,
    // meaning that the PWM pulse will cycle continuously.
    pwm->MCR = bit::Set(pwm->MCR, MatchControl::kPwm0Reset);

    // Enables PWM TC and PC for counting and enables PWM mode
    EnablePwm();

    // Enables PWM[channel] output
    uint32_t channel_bit_position =
        OutputControl::kEnableOutput.position + channel_.channel;
    pwm->PCR = bit::Set(pwm->PCR, channel_bit_position);

    channel_.pin.SetPinFunction(channel_.pin_function_code);

    return Status::kSuccess;
  }

  void SetDutyCycle(float duty_cycle) const override
  {
    SJ2_ASSERT_FATAL(0.0f <= duty_cycle && duty_cycle <= 1.0f,
                     "duty_cycle of Duty Cycle provided is out of bounds.");
    *GetMatchRegisters(channel_.channel) = CalculateDutyCycle(duty_cycle);
    // LER (Load Enable Register) sets the hardware up to load the new duty
    // cycle period in the next reset cycle after MR0 match. Hardware requires
    // this as a means ot prevent itself from glitching and changing pwm
    // instantaniously.
    channel_.peripheral.registers->LER |= (1 << channel_.channel);
  }

  float GetDutyCycle() const override
  {
    auto period     = static_cast<float>(*GetMatchRegisters(channel_.channel));
    auto max_period = static_cast<float>(*GetMatchRegisters(0));
    return period / max_period;
  }

  void SetFrequency(units::frequency::hertz_t frequency_hz) const override
  {
    SJ2_RETURN_IF(
        frequency_hz == 0_Hz,
        "Cannot set frequency to 0Hz! This call will have no effect!");

    auto & system = sjsu::SystemController::GetPlatformController();

    auto peripheral_frequency = system.GetClockRate(channel_.peripheral.id);
    // Get the current duty cycle so we can match it to the updated frequency.
    float previous_duty_cycle = GetDutyCycle();

    // In order to avoid PWM glitches, the PWM must be disabled while updating
    // the MR0 register. Doing this will reset all counters to 0 and allow us to
    // update MR0.
    EnablePwm(false);
    {
      auto new_frequency = (peripheral_frequency / frequency_hz).to<uint32_t>();
      *GetMatchRegisters(0) = new_frequency;
      SetDutyCycle(previous_duty_cycle);
    }
    EnablePwm(true);
  }

  /// Helper method for converting the match register 0 to a frequency in Hz.
  ///
  /// @return the current frequency of the PWM channel
  units::frequency::hertz_t GetFrequency() const
  {
    uint32_t match_register0         = *GetMatchRegisters(0);
    units::frequency::hertz_t result = 0_Hz;
    if (match_register0 != 0)
    {
      auto & system      = sjsu::SystemController::GetPlatformController();
      auto pwm_frequency = system.GetClockRate(channel_.peripheral.id);
      result = pwm_frequency / match_register0;
    }
    return result;
  }

 private:
  /// Helper method for enabling and disabling this PWM channel.
  ///
  /// @param enable - default to true, if set to false, will disable this PWM
  ///        channel.
  void EnablePwm(bool enable = true) const
  {
    volatile uint32_t * pwm_timer = &channel_.peripheral.registers->TCR;
    if (enable)
    {
      // Reset the Timer Counter
      *pwm_timer = bit::Set(*pwm_timer, Timer::kCounterReset);
      // Clear reset and allow timer to count
      *pwm_timer = bit::Clear(*pwm_timer, Timer::kCounterReset);
      // Enable PWM output
      *pwm_timer = bit::Set(*pwm_timer, Timer::kPwmEnable);
      // Enable counting
      *pwm_timer = bit::Set(*pwm_timer, Timer::kCounterEnable);
    }
    else
    {
      // Disable PWM output
      *pwm_timer = bit::Clear(*pwm_timer, Timer::kPwmEnable);
    }
  }

  /// Helper method to make getting a pointer to the Match Register 0 more
  /// readable in the code.
  ///
  /// @return a pointer to the match 0 register.
  volatile uint32_t * GetMatchRegisters(uint8_t match) const
  {
    switch (match)
    {
      case 0: return &channel_.peripheral.registers->MR0;
      case 1: return &channel_.peripheral.registers->MR1;
      case 2: return &channel_.peripheral.registers->MR2;
      case 3: return &channel_.peripheral.registers->MR3;
      case 4: return &channel_.peripheral.registers->MR4;
      case 5: return &channel_.peripheral.registers->MR5;
      case 6: return &channel_.peripheral.registers->MR6;
      default: return &channel_.peripheral.registers->MR0;
    }
  }
  /// Converts a percent value from 0.0f to 1.0f to the closest approximate
  /// match register value and returns that value.
  ///
  /// @param duty_cycle_percent - value from 0.0f to 1.0f indicating the duty
  ///        cycle precent.
  /// @return the nearest approximate match register value to achieve the
  ///         supplied duty cycle.
  uint32_t CalculateDutyCycle(float duty_cycle_percent) const
  {
    float pwm_period = static_cast<float>(*GetMatchRegisters(0));
    return static_cast<uint32_t>(duty_cycle_percent * pwm_period);
  }
  /// Reference to a const channel description for this instance of the PWM
  /// driver.
  const Channel_t & channel_;
};
}  // namespace lpc40xx
}  // namespace sjsu
