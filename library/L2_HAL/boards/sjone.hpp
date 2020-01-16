#pragma once

#include "L1_Peripheral/lpc17xx/adc.hpp"
#include "L1_Peripheral/lpc17xx/gpio.hpp"
#include "L1_Peripheral/lpc17xx/i2c.hpp"
#include "L1_Peripheral/lpc17xx/spi.hpp"
#include "L2_HAL/memory/sd.hpp"
#include "L2_HAL/sensors/environment/light/temt6000x01.hpp"
#include "L2_HAL/sensors/environment/temperature/tmp102.hpp"
#include "L2_HAL/sensors/movement/accelerometer/mma8452q.hpp"

/// Namespace for SJOne board peripheral and device definitions
struct sjone  // NOLINT
{
  /// Predefined SJOne spi0 peripheral
  inline static sjsu::lpc17xx::Spi spi0 =
      sjsu::lpc17xx::Spi(sjsu::lpc17xx::SpiBus::kSpi0);
  /// Predefined SJOne spi1 peripheral
  inline static sjsu::lpc17xx::Spi spi1 =
      sjsu::lpc17xx::Spi(sjsu::lpc17xx::SpiBus::kSpi1);

  /// Predefined SJOne i2c0 peripheral
  inline static sjsu::lpc17xx::I2c i2c0 =
      sjsu::lpc17xx::I2c(sjsu::lpc17xx::I2cBus::kI2c0);
  /// Predefined SJOne i2c1 peripheral
  inline static sjsu::lpc17xx::I2c i2c1 =
      sjsu::lpc17xx::I2c(sjsu::lpc17xx::I2cBus::kI2c1);
  /// Predefined SJOne i2c2 peripheral
  inline static sjsu::lpc17xx::I2c i2c2 =
      sjsu::lpc17xx::I2c(sjsu::lpc17xx::I2cBus::kI2c2);

  /// Predefined SJOne led0 peripheral
  inline static sjsu::lpc40xx::Gpio led0 = sjsu::lpc40xx::Gpio(1, 0);
  /// Predefined SJOne led1 peripheral
  inline static sjsu::lpc40xx::Gpio led1 = sjsu::lpc40xx::Gpio(1, 1);
  /// Predefined SJOne led2 peripheral
  inline static sjsu::lpc40xx::Gpio led2 = sjsu::lpc40xx::Gpio(1, 4);
  /// Predefined SJOne led3 peripheral
  inline static sjsu::lpc40xx::Gpio led3 = sjsu::lpc40xx::Gpio(1, 8);

  /// Predefined SJOne adc2 peripheral
  inline static sjsu::lpc17xx::Adc adc2 =
      sjsu::lpc17xx::Adc(sjsu::lpc17xx::AdcChannel::kChannel2);

  /// @returns fully constructed onboard Mma8452q device driver
  [[gnu::always_inline]] inline static sjsu::Mma8452q & Accelerometer()
  {
    static sjsu::Mma8452q accelerometer(i2c2);
    return accelerometer;
  }

  /// @returns fully constructed onboard Sd device driver
  [[gnu::always_inline]] inline static sjsu::Sd & SdCard()
  {
    static sjsu::lpc17xx::Gpio sd_cs = sjsu::lpc17xx::Gpio(2, 6);
    static sjsu::Sd sd(spi1, sd_cs);
    return sd;
  }

  /// @returns fully constructed onboard Tmp102 device driver
  [[gnu::always_inline]] inline static sjsu::Tmp102 & Temperature()
  {
    static sjsu::Tmp102 tmp102(i2c2);
    return tmp102;
  }

  /// @returns fully constructed onboard Temt6000x01 device driver
  [[gnu::always_inline]] inline static sjsu::Temt6000x01 & LightSensor()
  {
    // The LPC176x/5x ADC has a reference voltage of 3.3V.
    constexpr units::voltage::volt_t kAdcReferenceVoltage = 3.3_V;
    // A 10kOhm pull-down resistor is used on the SJOne board.
    constexpr units::impedance::ohm_t kPullDownResistance = 10_kOhm;
    static sjsu::Temt6000x01 temt6000(
        adc2, kAdcReferenceVoltage, kPullDownResistance);
    return temt6000;
  }
};
