// Tests for the St7066u Parallel LCD Driver class.
#include "L2_HAL/displays/lcd/st7066u.hpp"
#include "L2_HAL/io/parallel_bus.hpp"
#include "L4_Testing/testing_frameworks.hpp"

namespace sjsu
{
EMIT_ALL_METHODS(St7066u);

TEST_CASE("Testing St7066u Parallel LCD Driver")
{
  Mock<Gpio> mock_rs;  // RS: Register Select
  Mock<Gpio> mock_rw;  // RW: Read / Write
  Mock<Gpio> mock_e;   // E   Chip Enable
  Mock<ParallelBus> mock_data_bus;

  Fake(Method(mock_rs, SetDirection), Method(mock_rs, Set));
  Fake(Method(mock_rw, SetDirection), Method(mock_rw, Set));
  Fake(Method(mock_e, SetDirection), Method(mock_e, Set));
  Fake(Method(mock_data_bus, Initialize), Method(mock_data_bus, SetDirection),
       Method(mock_data_bus, Write));

  const St7066u kFourBitBusLcd(
      St7066u::BusMode::kFourBit, St7066u::DisplayMode::kSingleLine,
      St7066u::FontStyle::kFont5x8, mock_rs.get(), mock_rw.get(), mock_e.get(),
      mock_data_bus.get());
  const St7066u kEightBitBusLcd(
      St7066u::BusMode::kEightBit, St7066u::DisplayMode::kSingleLine,
      St7066u::FontStyle::kFont5x8, mock_rs.get(), mock_rw.get(), mock_e.get(),
      mock_data_bus.get());

  SECTION("Initialize")
  {
    constexpr St7066u::Command kDefaultConfiguration =
        St7066u::Command::kDefaultDisplayConfiguration;
    constexpr St7066u::DisplayMode kDisplayMode =
        St7066u::DisplayMode::kSingleLine;
    constexpr St7066u::FontStyle kFontStyle = St7066u::FontStyle::kFont5x8;
    St7066u::BusMode bus_mode;
    /// The expected bytes that are sent for initialization.
    std::array<uint8_t, 6> expected_data;
    size_t write_count = 0;

    When(Method(mock_data_bus, Write))
        .AlwaysDo([&expected_data, &write_count](uint32_t data) {
          INFO("write_count: " << write_count);
          INFO("data: " << data);
          CHECK(data == expected_data[write_count]);
          write_count++;
        });

    SECTION("With 4-bit Bus")
    {
      bus_mode                         = St7066u::BusMode::kFourBit;
      const uint8_t kConfigurationData = Value(kDefaultConfiguration) |
                                         Value(bus_mode) | Value(kDisplayMode) |
                                         Value(kFontStyle);
      // For 4-bit bus mode, the most significant nibble is sent first followed
      // by the least significant nibble.
      //
      // The data will be checked in the following order in the AlwaysDo() of
      // the stubbed Write() function.
      expected_data = {
        static_cast<uint8_t>(kConfigurationData >> 4),
        static_cast<uint8_t>(kConfigurationData & 0xF),
        0x0,
        Value(St7066u::Command::kTurnDisplayOn),
        0x0,
        Value(St7066u::Command::kClearDisplay),
      };
      kFourBitBusLcd.Initialize();
    }

    SECTION("With 8-bit Bus")
    {
      bus_mode      = St7066u::BusMode::kEightBit;
      expected_data = {
        static_cast<uint8_t>(Value(kDefaultConfiguration) | Value(bus_mode) |
                             Value(kDisplayMode) | Value(kFontStyle)),
        Value(St7066u::Command::kTurnDisplayOn),
        Value(St7066u::Command::kClearDisplay),
      };
      kEightBitBusLcd.Initialize();
    }

    Verify(Method(mock_rs, SetDirection).Using(Gpio::Direction::kOutput),
           Method(mock_rw, SetDirection).Using(Gpio::Direction::kOutput),
           Method(mock_e, SetDirection).Using(Gpio::Direction::kOutput),
           Method(mock_e, Set).Using(Gpio::kHigh),
           Method(mock_data_bus, Initialize),
           Method(mock_data_bus, SetDirection).Using(Gpio::Direction::kOutput));
  }

  SECTION("Write")
  {
    constexpr uint8_t kExpectedByte = 0b1010'1010;
    St7066u lcd                     = kEightBitBusLcd;

    lcd.Write(St7066u::WriteOperation::kCommand, kExpectedByte);
    Verify(Method(mock_rs, Set)
               .Using(Gpio::State(St7066u::WriteOperation::kCommand)),
           Method(mock_rw, Set).Using(Gpio::State::kLow),
           Method(mock_data_bus, Write).Using(kExpectedByte),
           Method(mock_e, Set).Using(Gpio::State::kLow),
           Method(mock_e, Set).Using(Gpio::State::kHigh));

    lcd.Write(St7066u::WriteOperation::kData, kExpectedByte);
    Verify(
        Method(mock_rs, Set).Using(Gpio::State(St7066u::WriteOperation::kData)),
        Method(mock_rw, Set).Using(Gpio::State::kLow),
        Method(mock_data_bus, Write).Using(kExpectedByte),
        Method(mock_e, Set).Using(Gpio::State::kLow),
        Method(mock_e, Set).Using(Gpio::State::kHigh));
  }

  SECTION("Write Command using 4-bit Bus")
  {
    constexpr Gpio::State kExpectedRegisterSelect =
        Gpio::State(St7066u::WriteOperation::kCommand);
    uint8_t expected_data;
    St7066u lcd = kFourBitBusLcd;

    SECTION("ClearDisplay")
    {
      expected_data = Value(St7066u::Command::kClearDisplay);
      lcd.ClearDisplay();
    }
    SECTION("SetDisplayOn")
    {
      SECTION("true")
      {
        expected_data = Value(St7066u::Command::kTurnDisplayOn);
        lcd.SetDisplayOn();
      }
      SECTION("false")
      {
        expected_data = Value(St7066u::Command::kTurnDisplayOff);
        lcd.SetDisplayOn(false);
      }
    }
    SECTION("SetCursorHidden")
    {
      SECTION("false")
      {
        expected_data = Value(St7066u::Command::kTurnCursorOn);
        lcd.SetCursorHidden(false);
      }
      SECTION("true")
      {
        expected_data = Value(St7066u::Command::kTurnDisplayOn);
        lcd.SetCursorHidden();
      }
    }
    SECTION("SetCursorDirection")
    {
      SECTION("kBackward")
      {
        expected_data = Value(St7066u::Command::kCursorDirectionBackward);
        lcd.SetCursorDirection(St7066u::CursorDirection::kBackward);
      }
      SECTION("kForward")
      {
        expected_data = Value(St7066u::Command::kCursorDirectionForward);
        lcd.SetCursorDirection(St7066u::CursorDirection::kForward);
      }
    }
    SECTION("SetCursorPosition")
    {
      constexpr uint8_t kLineNumber = 2;
      constexpr uint8_t kPosition   = 0x12;
      // Address should be 0x94 + 0x12 = 0xA6
      expected_data = Value(St7066u::Command::kDisplayLineAddress2) + kPosition;
      lcd.SetCursorPosition(
          St7066u::CursorPosition_t{ kLineNumber, kPosition });
    }
    SECTION("Reset Cursor Position")
    {
      expected_data = Value(St7066u::Command::kResetCursor);
      lcd.ResetCursorPosition();
    }

    Verify(Method(mock_rs, Set).Using(kExpectedRegisterSelect),
           Method(mock_rw, Set).Using(Gpio::State::kLow),
           Method(mock_data_bus, Write).Using(expected_data >> 4),
           Method(mock_e, Set).Using(Gpio::State::kLow),
           Method(mock_e, Set).Using(Gpio::State::kHigh));
    Verify(Method(mock_rs, Set).Using(kExpectedRegisterSelect),
           Method(mock_rw, Set).Using(Gpio::State::kLow),
           Method(mock_data_bus, Write).Using(expected_data & 0xF),
           Method(mock_e, Set).Using(Gpio::State::kLow),
           Method(mock_e, Set).Using(Gpio::State::kHigh));
  }

  SECTION("Write Command using 8-bit Bus")
  {
    constexpr Gpio::State kExpectedRegisterSelect =
        Gpio::State(St7066u::WriteOperation::kCommand);
    uint8_t expected_data;
    St7066u lcd = kEightBitBusLcd;

    SECTION("ClearDisplay")
    {
      expected_data = Value(St7066u::Command::kClearDisplay);
      lcd.ClearDisplay();
    }
    SECTION("SetDisplayOn")
    {
      SECTION("true")
      {
        expected_data = Value(St7066u::Command::kTurnDisplayOn);
        lcd.SetDisplayOn();
      }
      SECTION("false")
      {
        expected_data = Value(St7066u::Command::kTurnDisplayOff);
        lcd.SetDisplayOn(false);
      }
    }
    SECTION("SetCursorHidden")
    {
      SECTION("false")
      {
        expected_data = Value(St7066u::Command::kTurnCursorOn);
        lcd.SetCursorHidden(false);
      }
      SECTION("true")
      {
        expected_data = Value(St7066u::Command::kTurnDisplayOn);
        lcd.SetCursorHidden();
      }
    }
    SECTION("SetCursorDirection")
    {
      SECTION("kBackward")
      {
        expected_data = Value(St7066u::Command::kCursorDirectionBackward);
        lcd.SetCursorDirection(St7066u::CursorDirection::kBackward);
      }
      SECTION("kForward")
      {
        expected_data = Value(St7066u::Command::kCursorDirectionForward);
        lcd.SetCursorDirection(St7066u::CursorDirection::kForward);
      }
    }
    SECTION("SetCursorPosition")
    {
      constexpr uint8_t kLineNumber = 2;
      constexpr uint8_t kPosition   = 0x12;
      // Address should be 0x94 + 0x12 = 0xA6
      expected_data = Value(St7066u::Command::kDisplayLineAddress2) + kPosition;
      lcd.SetCursorPosition(
          St7066u::CursorPosition_t{ kLineNumber, kPosition });
    }
    SECTION("Reset Cursor Position")
    {
      expected_data = Value(St7066u::Command::kResetCursor);
      lcd.ResetCursorPosition();
    }

    Verify(Method(mock_rs, Set).Using(kExpectedRegisterSelect),
           Method(mock_rw, Set).Using(Gpio::State::kLow),
           Method(mock_data_bus, Write).Using(expected_data),
           Method(mock_e, Set).Using(Gpio::State::kLow),
           Method(mock_e, Set).Using(Gpio::State::kHigh));
  }

  SECTION("Display Text")
  {
    const char kText[] = "Text String";

    SECTION("With 4-bit Bus")
    {
      kFourBitBusLcd.DisplayText(kText);
      for (uint8_t i = 0; i < strlen(kText); i++)
      {
        INFO("character: " << kText[i]);
        const uint8_t kExpectedHighNibble = kText[i] >> 4;
        const uint8_t kExpectedLowNibble  = kText[i] & 0xF;
        Verify(Method(mock_rs, Set)
                   .Using(Gpio::State(St7066u::WriteOperation::kCommand)),
               Method(mock_rw, Set).Using(Gpio::State::kLow),
               Method(mock_data_bus, Write).Using(kExpectedHighNibble),
               Method(mock_e, Set).Using(Gpio::State::kLow),
               Method(mock_e, Set).Using(Gpio::State::kHigh));
        Verify(Method(mock_rs, Set)
                   .Using(Gpio::State(St7066u::WriteOperation::kCommand)),
               Method(mock_rw, Set).Using(Gpio::State::kLow),
               Method(mock_data_bus, Write).Using(kExpectedLowNibble),
               Method(mock_e, Set).Using(Gpio::State::kLow),
               Method(mock_e, Set).Using(Gpio::State::kHigh));
      }
    }
    SECTION("With 8-bit Bus")
    {
      kEightBitBusLcd.DisplayText(kText);
      for (uint8_t i = 0; i < strlen(kText); i++)
      {
        INFO("character: " << kText[i]);
        Verify(Method(mock_rs, Set)
                   .Using(Gpio::State(St7066u::WriteOperation::kCommand)),
               Method(mock_rw, Set).Using(Gpio::State::kLow),
               Method(mock_data_bus, Write).Using(kText[i]),
               Method(mock_e, Set).Using(Gpio::State::kLow),
               Method(mock_e, Set).Using(Gpio::State::kHigh));
      }
    }
  }
}
}  // namespace sjsu
