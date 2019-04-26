#pragma once
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <initializer_list>

#include "config.hpp"

#include "L0_LowLevel/interrupt.hpp"
#include "L0_LowLevel/LPC40xx.h"
#include "L0_LowLevel/system_controller.hpp"
#include "L1_Drivers/pin.hpp"
#include "utility/enum.hpp"
#include "utility/log.hpp"
#include "utility/status.hpp"
#include "utility/time.hpp"

class I2cInterface
{
 public:
  enum Operation : uint8_t
  {
    kWrite = 0,
    kRead  = 1,
  };

  static constexpr uint32_t kI2cTimeout = 1000;  // units milliseconds

  struct Transaction_t
  {
    // Returns an 8-bit I2C address with the 0th bit set if the i2c operation
    // is kRead.
    constexpr uint8_t GetProperAddress()
    {
      uint8_t address_8bit = static_cast<uint8_t>(address << 1);
      if (operation == Operation::kRead)
      {
        address_8bit |= 1;
      }
      return address_8bit;
    }
    uint64_t timeout         = I2cInterface::kI2cTimeout;
    size_t out_length        = 0;
    size_t in_length         = 0;
    size_t position          = 0;
    const uint8_t * data_out = nullptr;
    uint8_t * data_in        = nullptr;
    Status status            = Status::kSuccess;
    Operation operation      = Operation::kWrite;
    uint8_t address          = 0xFF;
    bool repeated            = false;
    bool busy                = false;
  };

  virtual void Initialize() const                                           = 0;
  virtual Status Read(uint8_t address, uint8_t * destination, size_t length,
                      uint32_t timeout = kI2cTimeout) const                 = 0;
  virtual Status Write(uint8_t address, const uint8_t * destination,
                       size_t length, uint32_t timeout = kI2cTimeout) const = 0;
  virtual Status WriteThenRead(uint8_t address, const uint8_t * transmit,
                               size_t out_length, uint8_t * recieve,
                               size_t recieve_length,
                               uint32_t timeout = kI2cTimeout) const        = 0;
  Status WriteThenRead(uint8_t address, std::initializer_list<uint8_t> transmit,
                       uint8_t * recieve, size_t recieve_length,
                       uint32_t timeout = kI2cTimeout) const
  {
    return WriteThenRead(address, transmit.begin(), transmit.size(), recieve,
                         recieve_length, timeout);
  }
  Status Write(uint8_t address, std::initializer_list<uint8_t> data,
               uint32_t timeout = kI2cTimeout) const
  {
    return Write(address, data.begin(), data.size(), timeout);
  }
};

class I2c final : public I2cInterface, protected Lpc40xxSystemController
{
 public:
  // Bringing in I2cInterface's Write and WriteThenRead methods that use
  // std::initializer_list.
  using I2cInterface::Write;
  using I2cInterface::WriteThenRead;

  enum Control : uint32_t
  {
    kAssertAcknowledge = 1 << 2,  // AA
    kInterrupt         = 1 << 3,  // SI
    kStop              = 1 << 4,  // STO
    kStart             = 1 << 5,  // STA
    kInterfaceEnable   = 1 << 6   // I2EN
  };

  enum class MasterState : uint32_t
  {
    kBusError                          = 0x00,
    kStartCondition                    = 0x08,
    kRepeatedStart                     = 0x10,
    kSlaveAddressWriteSentRecievedAck  = 0x18,
    kSlaveAddressWriteSentRecievedNack = 0x20,
    kTransmittedDataRecievedAck        = 0x28,
    kTransmittedDataRecievedNack       = 0x30,
    kArbitrationLost                   = 0x38,
    kSlaveAddressReadSentRecievedAck   = 0x40,
    kSlaveAddressReadSentRecievedNack  = 0x48,
    kRecievedDataRecievedAck           = 0x50,
    kRecievedDataRecievedNack          = 0x58,
    kOwnAddressReceived                = 0xA0,
    kDoNothing                         = 0xF8
  };

  struct PartialBus_t
  {
    LPC_I2C_TypeDef * registers;
    PeripheralID peripheral_power_id;
    IRQn_Type irq_number;
    Transaction_t & transaction;
    const PinInterface & sda_pin;
    const PinInterface & scl_pin;
    uint8_t pin_function_id;
  };

  struct Bus_t
  {
    const PartialBus_t & bus;
    IsrPointer handler;
  };

  template <const PartialBus_t & i2c>
  static void I2cHandler()
  {
    I2cHandler(i2c);
  }

  struct Bus  // NOLINT
  {
   private:
    inline static const Pin kI2c0SdaPin = Pin::CreatePin<0, 0>();
    inline static const Pin kI2c0SclPin = Pin::CreatePin<0, 1>();
    inline static const Pin kI2c1SdaPin = Pin::CreatePin<1, 30>();
    inline static const Pin kI2c1SclPin = Pin::CreatePin<1, 31>();
    inline static const Pin kI2c2SdaPin = Pin::CreatePin<0, 10>();
    inline static const Pin kI2c2SclPin = Pin::CreatePin<0, 11>();
    // UM10562: Chapter 7: LPC408x/407x I/O configuration page 133
    inline static Transaction_t transaction_i2c0;
    inline static const PartialBus_t kI2c0Partial = {
      .registers           = LPC_I2C0,
      .peripheral_power_id = Lpc40xxSystemController::Peripherals::kI2c0,
      .irq_number          = I2C0_IRQn,
      .transaction         = transaction_i2c0,
      .sda_pin             = kI2c0SdaPin,
      .scl_pin             = kI2c0SclPin,
      .pin_function_id     = 0b010,
    };

    inline static Transaction_t transaction_i2c1;
    inline static const PartialBus_t kI2c1Partial = {
      .registers           = LPC_I2C1,
      .peripheral_power_id = Lpc40xxSystemController::Peripherals::kI2c1,
      .irq_number          = I2C1_IRQn,
      .transaction         = transaction_i2c1,
      .sda_pin             = kI2c1SdaPin,
      .scl_pin             = kI2c1SclPin,
      .pin_function_id     = 0b011,
    };

    inline static Transaction_t transaction_i2c2;
    inline static const PartialBus_t kI2c2Partial = {
      .registers           = LPC_I2C2,
      .peripheral_power_id = Lpc40xxSystemController::Peripherals::kI2c2,
      .irq_number          = I2C2_IRQn,
      .transaction         = transaction_i2c2,
      .sda_pin             = kI2c2SdaPin,
      .scl_pin             = kI2c2SclPin,
      .pin_function_id     = 0b010,
    };

   public:
    inline static const Bus_t kI2c0 = { .bus     = kI2c0Partial,
                                        .handler = I2cHandler<kI2c0Partial> };

    inline static const Bus_t kI2c1 = { .bus     = kI2c1Partial,
                                        .handler = I2cHandler<kI2c1Partial> };

    inline static const Bus_t kI2c2 = { .bus     = kI2c2Partial,
                                        .handler = I2cHandler<kI2c2Partial> };
  };

  static void I2cHandler(const PartialBus_t & i2c)
  {
    MasterState state   = MasterState(i2c.registers->STAT);
    uint32_t clear_mask = 0;
    uint32_t set_mask   = 0;
    switch (state)
    {
      case MasterState::kBusError:  // 0x00
      {
        i2c.transaction.status = Status::kBusError;
        set_mask               = Control::kAssertAcknowledge | Control::kStop;
        break;
      }
      case MasterState::kStartCondition:  // 0x08
      {
        i2c.registers->DAT = i2c.transaction.GetProperAddress();
        break;
      }
      case MasterState::kRepeatedStart:  // 0x10
      {
        i2c.transaction.operation = Operation::kRead;
        i2c.registers->DAT        = i2c.transaction.GetProperAddress();
        break;
      }
      case MasterState::kSlaveAddressWriteSentRecievedAck:  // 0x18
      {
        clear_mask = Control::kStart;
        if (i2c.transaction.out_length == 0)
        {
          i2c.transaction.busy   = false;
          i2c.transaction.status = Status::kSuccess;
          set_mask               = Control::kStop;
        }
        else
        {
          size_t position    = i2c.transaction.position++;
          i2c.registers->DAT = i2c.transaction.data_out[position];
        }
        break;
      }
      case MasterState::kSlaveAddressWriteSentRecievedNack:  // 0x20
      {
        clear_mask             = Control::kStart;
        i2c.transaction.busy   = false;
        i2c.transaction.status = Status::kDeviceNotFound;
        set_mask               = Control::kStop;
        break;
      }
      case MasterState::kTransmittedDataRecievedAck:  // 0x28
      {
        if (i2c.transaction.position >= i2c.transaction.out_length)
        {
          if (i2c.transaction.repeated)
          {
            // OR with 1 to set address as READ for the next transaction
            i2c.transaction.operation = Operation::kRead;
            i2c.transaction.position  = 0;
            set_mask                  = Control::kStart;
          }
          else
          {
            i2c.transaction.busy = false;
            set_mask             = Control::kStop;
          }
        }
        else
        {
          size_t position    = i2c.transaction.position++;
          i2c.registers->DAT = i2c.transaction.data_out[position];
        }
        break;
      }
      case MasterState::kTransmittedDataRecievedNack:  // 0x30
      {
        i2c.transaction.busy = false;
        set_mask             = Control::kStop;
        break;
      }
      case MasterState::kArbitrationLost:  // 0x38
      {
        set_mask = Control::kStart;
        break;
      }
      case MasterState::kSlaveAddressReadSentRecievedAck:  // 0x40
      {
        clear_mask = Control::kStart;
        if (i2c.transaction.in_length == 0)
        {
          set_mask = Control::kStop;
        }
        // If we only want 1 byte, make sure to nack that byte
        else if (i2c.transaction.in_length == 1)
        {
          clear_mask |= Control::kAssertAcknowledge;
        }
        // If we want more then 1 byte, make sure to ack the first byte
        else
        {
          set_mask = Control::kAssertAcknowledge;
        }
        break;
      }
      case MasterState::kSlaveAddressReadSentRecievedNack:  // 0x48
      {
        clear_mask             = Control::kStart;
        i2c.transaction.status = Status::kDeviceNotFound;
        i2c.transaction.busy   = false;
        set_mask               = Control::kStop;
        break;
      }
      case MasterState::kRecievedDataRecievedAck:  // 0x50
      {
        const size_t kBufferEnd = i2c.transaction.in_length;
        if (i2c.transaction.position < kBufferEnd)
        {
          const size_t kPosition = i2c.transaction.position;
          i2c.transaction.data_in[kPosition] =
              static_cast<uint8_t>(i2c.registers->DAT);
          i2c.transaction.position++;
        }
        // Check if the position has been pushed past the buffer length
        if (i2c.transaction.position + 1 >= kBufferEnd)
        {
          clear_mask           = Control::kAssertAcknowledge;
          i2c.transaction.busy = false;
        }
        else
        {
          set_mask = Control::kAssertAcknowledge;
        }
        break;
      }
      case MasterState::kRecievedDataRecievedNack:  // 0x58
      {
        i2c.transaction.busy = false;
        if (i2c.transaction.in_length != 0)
        {
          size_t position = i2c.transaction.position++;
          i2c.transaction.data_in[position] =
              static_cast<uint8_t>(i2c.registers->DAT);
        }
        set_mask = Control::kStop;
        break;
      }
      case MasterState::kDoNothing:  // 0xF8
      {
        break;
      }
      default:
      {
        clear_mask = Control::kStop;
        SJ2_ASSERT_FATAL(false, "Invalid I2C State Reached!!");
        break;
      }
    }
    // Clear I2C Interrupt flag
    clear_mask |= Control::kInterrupt;
    // Set register controls
    i2c.registers->CONSET = set_mask;
    i2c.registers->CONCLR = clear_mask;
  }

  // This defaults to I2C port 2
  explicit constexpr I2c(const Bus_t & bus) : i2c_(bus) {}

  void Initialize() const override
  {
    i2c_.bus.sda_pin.SetPinFunction(i2c_.bus.pin_function_id);
    i2c_.bus.scl_pin.SetPinFunction(i2c_.bus.pin_function_id);
    i2c_.bus.sda_pin.SetAsOpenDrain();
    i2c_.bus.scl_pin.SetAsOpenDrain();
    i2c_.bus.sda_pin.SetMode(PinInterface::Mode::kInactive);
    i2c_.bus.scl_pin.SetMode(PinInterface::Mode::kInactive);

    PowerUpPeripheral(i2c_.bus.peripheral_power_id);

    float peripheral_frequency = static_cast<float>(GetPeripheralFrequency());
    float scll = ((peripheral_frequency / 75'000.0f) / 2.0f) * 0.7f;
    float sclh = ((peripheral_frequency / 75'000.0f) / 2.0f) * 1.3f;

    i2c_.bus.registers->SCLL = static_cast<uint32_t>(scll);
    i2c_.bus.registers->SCLH = static_cast<uint32_t>(sclh);

    i2c_.bus.registers->CONCLR = Control::kAssertAcknowledge | Control::kStart |
                                 Control::kStop | Control::kInterrupt;
    i2c_.bus.registers->CONSET = Control::kInterfaceEnable;

    RegisterIsr(i2c_.bus.irq_number, i2c_.handler, true);
  }

  Status Read(uint8_t address, uint8_t * data, size_t length,
              uint32_t timeout = kI2cTimeout) const override
  {
    i2c_.bus.transaction = { .timeout    = timeout,
                             .out_length = 0,
                             .in_length  = length,
                             .position   = 0,
                             .data_out   = nullptr,
                             .data_in    = data,
                             .status     = Status::kSuccess,
                             .operation  = Operation::kRead,
                             .address    = address,
                             .repeated   = false,
                             .busy       = true };

    i2c_.bus.registers->CONSET = Control::kStart;
    return BlockUntilFinished();
  }

  Status Write(uint8_t address, const uint8_t * data, size_t length,
               uint32_t timeout = kI2cTimeout) const override
  {
    i2c_.bus.transaction = { .timeout    = timeout,
                             .out_length = length,
                             .in_length  = 0,
                             .position   = 0,
                             .data_out   = data,
                             .data_in    = nullptr,
                             .status     = Status::kSuccess,
                             .operation  = Operation::kWrite,
                             .address    = address,
                             .repeated   = false,
                             .busy       = true };

    i2c_.bus.registers->CONSET = Control::kStart;
    return BlockUntilFinished();
  }
  Status WriteThenRead(uint8_t address, const uint8_t * transmit,
                       size_t out_length, uint8_t * recieve,
                       size_t recieve_length,
                       uint32_t timeout = kI2cTimeout) const override
  {
    i2c_.bus.transaction = { .timeout    = timeout,
                             .out_length = out_length,
                             .in_length  = recieve_length,
                             .position   = 0,
                             .data_out   = transmit,
                             .data_in    = recieve,
                             .status     = Status::kSuccess,
                             .operation  = Operation::kWrite,
                             .address    = address,
                             .repeated   = true,
                             .busy       = true };

    i2c_.bus.registers->CONSET = Control::kStart;
    return BlockUntilFinished();
  }

  const Transaction_t GetTransactionInfo()
  {
    return i2c_.bus.transaction;
  }

  bool IsIntialized() const
  {
    return (i2c_.bus.registers->CONSET & Control::kInterfaceEnable);
  }

 protected:
  Status BlockUntilFinished() const
  {
  #if !defined(HOST_TEST)  // TODO(#440): Replace with if constexpr
    SJ2_ASSERT_FATAL(IsIntialized(),
                     "Attempted to use I2C, but peripheral was not "
                     "initialized! Be sure to run the Initialize() method "
                     "of this class, before using it.");
    auto wait_for_i2c_transaction = [this]() -> bool {
      return !i2c_.bus.transaction.busy;
    };
    Status status =
        Wait(i2c_.bus.transaction.timeout, wait_for_i2c_transaction);

    if (status == Status::kTimedOut)
    {
      // Abort I2C communication if this point is reached!
      i2c_.bus.registers->CONSET = Control::kAssertAcknowledge | Control::kStop;
      SJ2_ASSERT_WARNING(
          i2c_.bus.transaction.out_length == 0 ||
              i2c_.bus.transaction.in_length == 0,
          "I2C took too long to process and timed out! If the "
          "transaction needs more time, you may want to increase the "
          "timeout time.");
      i2c_.bus.transaction.status = Status::kTimedOut;
    }
    // Ensure that start is cleared before leaving this function
    i2c_.bus.registers->CONCLR = Control::kStart;
#endif  // !defined HOST_TEST
    return i2c_.bus.transaction.status;
  }
  const Bus_t & i2c_;
};
