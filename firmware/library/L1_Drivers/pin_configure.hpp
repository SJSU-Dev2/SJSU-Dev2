// PinConfigure abstracts the process of changing the mode and attributes of a
// pin on the LPC40xx series of chips.
//   Usage:
//      PinConfigure P0_0(0, 0);
//      P0_0.SetAsActiveLow();
//      P0_0.SetPinMode(PinConfigureInterface::PinMode::kPullUp);
#pragma once

#include <cstdio>

#include "L0_LowLevel/LPC40xx.h"
#include "L2_Utilities/macros.hpp"

class PinConfigureInterface
{
   public:
    // Source: "UM10562 LPC408x/407x User manual" table 83 page 132
    enum PinMode : uint8_t
    {
        kInactive = 0,
        kPullDown,
        kPullUp,
        kRepeater
    };
    virtual void SetPinFunction(uint8_t function)         = 0;
    virtual void SetPinMode(PinMode mode)                 = 0;
    virtual void EnableHysteresis(bool enable_hysteresis) = 0;
    virtual void SetAsActiveLow(bool set_as_active_low)   = 0;
    virtual void EnableFastMode(bool enable_fast_mode)    = 0;
    virtual void SetAsOpenDrain(bool set_as_open_drain)   = 0;
};

class PinConfigure : public PinConfigureInterface
{
   public:
    // Source: "UM10562 LPC408x/407x User manual" table 83 page 132
    enum PinBitMap : uint8_t
    {
        kFunction    = 0,
        kMode        = 3,
        kHysteresis  = 5,
        kInputInvert = 6,
        kSlew        = 9,
        kOpenDrain   = 10
    };
    SJ2_PACKED(struct) PinMap_t
    {
        using pin_config_register_t = uint32_t;
        pin_config_register_t _register[6][32];
    };

    static PinMap_t * pin_map;
    // Compile time validating PinConfigure factory.
    // Will test the port and pin variables to make sure they are within bounds
    // of the pin_config_register.
    template <unsigned port, unsigned pin>
    static constexpr PinConfigure CreatePinConfigure()
    {
        static_assert(port <= 5, "Port must be between 0 and 5");
        static_assert(pin <= 31, "Pin must be between 0 and 31");
        static_assert(port == 5 && pin <= 4,
            "For port 5, the pin number must be equal to or below 4");
        return PinConfigure(port, pin);
    }
    constexpr PinConfigure(const uint8_t _port, const uint8_t _pin)
        : port(_port), pin(_pin) { }
    void SetPinFunction(uint8_t function) final override
    {
        pin_map->_register[port][pin] =
            BitPlace(pin_map->_register[port][pin], PinBitMap::kFunction,
                     function & 0b111, 3);
    }
    void SetPinMode(PinConfigureInterface::PinMode mode) final override
    {
        pin_map->_register[port][pin] = BitPlace(
            pin_map->_register[port][pin], PinBitMap::kMode, mode & 0b11, 2);
    }
    void EnableHysteresis(bool enable_hysteresis = true) final override
    {
        pin_map->_register[port][pin] =
            BitPlace(pin_map->_register[port][pin], PinBitMap::kHysteresis,
                     enable_hysteresis, 1);
    }
    void SetAsActiveLow(bool set_as_active_low = true) final override
    {
        pin_map->_register[port][pin] =
            BitPlace(pin_map->_register[port][pin], PinBitMap::kInputInvert,
                     set_as_active_low, 1);
    }
    void EnableFastMode(bool enable_fast_mode = true) final override
    {
        pin_map->_register[port][pin] =
            BitPlace(pin_map->_register[port][pin], PinBitMap::kSlew,
                     enable_fast_mode, 1);
    }
    void SetAsOpenDrain(bool set_as_open_drain = true) final override
    {
        pin_map->_register[port][pin] =
            BitPlace(pin_map->_register[port][pin], PinBitMap::kOpenDrain,
                     set_as_open_drain, 1);
    }
    inline uint32_t BitPlace(uint32_t target, uint32_t position, uint32_t value,
                      uint32_t value_width)
    {
        // Generate mask with all 1s
        uint32_t mask = 0xFFFFFFFF >> (32 - value_width);
        target &= ~(mask << position);
        target |= value << position;
        return target;
    }

   private:
    const uint8_t port;
    const uint8_t pin;
};
