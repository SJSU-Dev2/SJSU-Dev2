#pragma once

#include <cmath>
#include <cstdint>
#include <type_traits>

#include "L1_Peripheral/gpio.hpp"
#include "L1_Peripheral/spi.hpp"
#include "L1_Peripheral/storage.hpp"
#include "L1_Peripheral/inactive.hpp"
#include "utility/log.hpp"
#include "utility/crc.hpp"

namespace sjsu::experimental
{
/// SD card controller utilizing the SPI peripheral
class Sd : public Storage
{
 public:
  /// This value was found through experimentation. The suggested value (found
  /// via research) is to wait at least 1-8 bytes after sending a command's CRC
  /// before expecting a response from the card.
  /// See https://luckyresistor.me/cat-protector/software/sdcard-2/
  static constexpr uint8_t kRetryLimit = 250;

  /// Enforcing block-size cross-compatibility
  static constexpr uint16_t kBlockSize = 512;

  /// Table holding CRC8 tokens used to verify SD card transactions.
  static constexpr sjsu::crc::CrcTableConfig_t<uint8_t> kCrcTable8 =
      sjsu::crc::GenerateCrc7Table<uint8_t>();

  /// Table holding CRC16 tokens used to verify SD card transactions.
  static constexpr sjsu::crc::CrcTableConfig_t<uint16_t> kCrcTable16 =
      sjsu::crc::GenerateCrc16Table();

  /// Default SPI frequency for SD card communication
  static constexpr units::frequency::hertz_t kDefaultSpiFrequency = 12_MHz;

  /// A response frame struct to contain the various responses sent by the card
  /// after commands are issued (response type and length depend on the command
  /// sent)
  struct Response_t
  {
    /// Array buffer to hold the response bytes
    std::array<uint8_t, 8> byte;
    /// The number of bytes in the response.
    uint32_t length;
  };

  /// Enumerator for response type codes (See Physical Layer V6.00 p.225
  enum class ResponseType
  {
    kR1,
    kR1b,
    kR2,
    kR3,
    kR4,
    kR5,
    kR6,
    kR7
  };

  static constexpr bit::Mask kIllegalCommand = bit::CreateMaskFromRange(2);
  static constexpr bit::Mask kIdle           = bit::CreateMaskFromRange(0);

  /// Enumerator for SD Card Capacity type codes
  enum class Type
  {
    kSDSC,  // up to 2GB
    kSDHC,  // up to 32GB
    kSDXC   // up to 256GB
  };

  /// Bit shift counts provided to allow easy checking of the OCR flag bits for
  /// the card's operating conditions
  enum class Ocr
  {
    kIsDualVoltage = 7,  // OCR bit 7: Supports both Low and High
                         // Voltage Ranges
    kIs27vTo28v = 15,    // OCR bit 15: Supports 2.7V-2.8V
    kIs28vTo29v = 16,    // OCR bit 16: Supports 2.8V-2.9V
    kIs29vTo30v = 17,    // OCR bit 17: Supports 2.9V-3.0V
    kIs30vTo31v = 18,    // OCR bit 18: Supports 3.0V-3.1V
    kIs31vTo32v = 19,    // OCR bit 19: Supports 3.1V-3.2V
    kIs32vTo33v = 20,    // OCR bit 20: Supports 3.2V-3.3V
    kIs33vTo34v = 21,    // OCR bit 21: Supports 3.3V-3.4V
    kIs34vTo35v = 22,    // OCR bit 22: Supports 3.4V-3.5V
    kIs35vTo36v = 23,    // OCR bit 23: Supports 3.5V-3.6V
    kIs18vOk    = 24,    // OCR bit 24: Switching to 1.8V is
                         // accepted (S18A)
    kIsUhs2 = 29,        // OCR bit 29: Card is UHS-II Compatible
    kCcs    = 30,        // OCR bit 30: Card Capacity Status (used)
                         // to determine if a card is SDSC or not
    kPwrUpComplete = 31  // OCR bit 31: Card Power Up Status (i.e.
                         // if card has finished the power cycle
                         // process)
  };

  /// The start bits for every command
  static constexpr uint32_t kCommandBase = 0x40;

  /// A collection of SPI-supported command codes to be used with SendCommand()
  ///
  /// @note OR of b0100_0000 is meant to conform with the SD protocol
  ///       (i.e. first bits must be b01)
  enum class Command
  {
    kGarbage = 0xFF,                 // "SendGarbage" command; instructs
                                     // SendCommand() to send 0xFFFFFFFF as data
                                     // and even set the checksum as 0xFF
    kReset = kCommandBase | 0,       // CMD0: reset the sd card (force it to go
                                     // to the idle state)
    kInit  = kCommandBase | 1,       // CMD1: starts an initiation of the card
    kGetOp = kCommandBase | 8,       // CMD8: request the sd card's support of
                                     // the provided host's voltage ranges
    kGetCsd = kCommandBase | 9,      // CMD9: request the sd card's CSD
                                     // (card-specific data) register
    kStopTrans = kCommandBase | 12,  // CMD12: terminates a multi-block read or
                                     // write operation
    kGetStatus = kCommandBase | 13,  // CMD13: get status register
    kChgBlkLen = kCommandBase | 16,  // CMD16: change block length (only
                                     // effective in SDSC cards; SDHC/SDXC
                                     // cards are locked to 512-byte blocks)
    kReadSingle = kCommandBase | 17,   // CMD17: read a single block of data
    kReadMulti  = kCommandBase | 18,   // CMD18: read many blocks of data until
                                       // a "CMD12" frame is sent
    kWriteSingle = kCommandBase | 24,  // CMD24: write a single block of data
    kWriteMulti  = kCommandBase | 25,  // CMD25: write many blocks of data until
                                       // a "CMD12" frame is sent
    kDelFrom = kCommandBase | 32,      // CMD32: set address of the start block
                                       // for deletion
    kDelTo = kCommandBase | 33,    // CMD33: set address of the end block for
                                   // deletion
    kDel = kCommandBase | 38,      // CMD38: begin deletion from the block
                                   // range specified by the vector
                                   // [DEL_FROM : DEL_TO]
    kAcBegin = kCommandBase | 55,  // CMD55: signals the start of an
                                   // application-specific command
    kGetOcr = kCommandBase | 58,   // CMD58: request data from the
                                   // operational conditions register
    kAcInit = kCommandBase | 41    // CMD41: application-specific version of
                                   // CMD1 (must precede with CMD55)
  };

  /// Enumerations to allow CS keep-alive during command byte exchanges
  enum class KeepAlive : bool
  {
    kYes = true,
    kNo  = false
  };

  /// Card Data Buffer
  struct CsdBuffer_t
  {
    /// Length of the CSD buffer +2 for CRC
    static constexpr size_t kBytesInCsdRegister = 16 + 2;
    /// CSD contents
    std::array<uint8_t, kBytesInCsdRegister> byte;
    /// Determines if hte CSD contents are valid.
    Status status = Status::kSuccess;
  };

  /// Structure for recording information about the SD Card
  struct CardInfo_t
  {
    /// Store OCR information
    union {
      uint32_t dWord;
      uint16_t word[2];
      uint8_t byte[4];
    } ocr;
    /// Define the storage type of the SD card
    Type type;
    /// Container for the CSD registers contents
    CsdBuffer_t csd;
  };

  /// @param spi         - spi peripheral connected to an SD card.
  /// @param chip_select - gpio connected to the chip select pin of the SD card.
  /// @param card_detect - gpio connected to the card detect pin of the SD card
  ///                      socket.
  /// @param spi_clock_rate - clock frequency for the SPI communication with
  ///                         the SD card.
  /// @param active_level - the active voltage level of the card detect signal.
  constexpr Sd(Spi & spi,
               Gpio & chip_select,
               Gpio & card_detect,
               units::frequency::hertz_t spi_clock_rate = kDefaultSpiFrequency,
               Gpio::State active_level                 = Gpio::State::kLow)
      : spi_(spi),
        chip_select_(chip_select),
        card_detect_(card_detect),
        card_detect_active_level_(active_level),
        spi_clock_rate_(spi_clock_rate),
        sd_{}
  {
  }

  Storage::Type GetMemoryType() override
  {
    return Storage::Type::kSD;
  }

  Status Initialize() override
  {
    chip_select_.SetDirection(Gpio::Direction::kOutput);
    chip_select_.Set(Gpio::State::kHigh);
    card_detect_.SetDirection(Gpio::Direction::kInput);

    spi_.Initialize();
    /// Defaulting to 400 kHz as that is the frequency that allows mounting to
    /// be possible.
    spi_.SetClock(400_kHz);
    spi_.SetDataSize(Spi::DataSize::kEight);
    return Status::kSuccess;
  }

  bool IsMediaPresent() override
  {
    bool card_detect_state = card_detect_.Read();

    if (card_detect_state == true &&
        card_detect_active_level_ == Gpio::State::kHigh)
    {
      return true;
    }

    if (card_detect_state == false &&
        card_detect_active_level_ == Gpio::State::kLow)
    {
      return true;
    }

    return false;
  }

  sjsu::Status Enable() override
  {
    Status status = Mount();
    if (!IsOk(status))
    {
      SendCommand(Command::kGarbage, 0xFFFFFFFF, KeepAlive::kNo);
    }
    return status;
  }

  sjsu::Status Disable() override
  {
    return Status::kNotImplemented;
  }

  /// Assumes that write protection is never enabled.
  bool IsReadOnly() override
  {
    return false;
  }

  units::data::byte_t GetBlockSize() override
  {
    return units::data::byte_t{ kBlockSize };
  }

  units::data::byte_t GetCapacity() override
  {
    // The c_size register's contents can be found in bits [48:69]
    constexpr bit::Mask kCSizeMask = { .position = 56, .width = 21 };
    uint32_t c_size = bit::StreamExtract<uint32_t>(sd_.csd.byte, kCSizeMask);
    LogDebug("c_size = 0x%08X", c_size);

    // Calculate the actual card size with the equation = (c_size + 1) * 512kB
    units::data::byte_t card_size = (c_size + 1) * 512_kB;

    return card_size;
  };

  sjsu::Status Erase(uint32_t block_address, size_t block_count) override
  {
    EraseBlock(block_address, block_count);
    return sjsu::Status::kSuccess;
  }

  sjsu::Status Write(uint32_t block_address,
                     const void * data,
                     size_t size) override
  {
    // Convert the void* to a uint8_t* so we can byte access it
    const uint8_t * data_ptr = reinterpret_cast<const uint8_t *>(data);

    // A counter that counts the number of bytes read. Is used to determine when
    // to stop this algorithm.
    size_t bytes_written = 0;

    // Flag used to exit the block read for-loop when the transfer has
    // completed.
    bool finished = false;

    for (size_t block_offset = 0; !finished; block_offset++)
    {
      // Create a block that will contain the data to be written to the SD card.
      // This block ensures that we have a 512 sized buffer to supply to
      // WriteBlock as the input data's size may not be equal to the size of a
      // block.
      Block_t block;

      // Fill block with 1s. This is important as SD cards (and most flash
      // memories) consider 1s to be in an erased stated.
      std::fill(block.byte.begin(), block.byte.end(), 0xFF);

      // Start copying the contents of `data` into the block and exit early if
      // we finish writting bytes.
      for (uint32_t index = 0; index < kBlockSize; index++)
      {
        block.byte[index] = data_ptr[bytes_written++];
        if (bytes_written >= size)
        {
          finished = true;
          break;
        }
      }

      // Set the CRC bytes of the block before transmission
      block.SetCrcBytes();

      // Convert the block address into a byte address
      uint32_t block_byte_address = (block_address + block_offset) * kBlockSize;
      WriteBlock(block_byte_address, block);
    }

    return Status::kSuccess;
  }

  sjsu::Status Read(uint32_t block_address, void * data, size_t size) override
  {
    // Convert the void* to a uint8_t* so we can byte access it
    uint8_t * data_ptr = reinterpret_cast<uint8_t *>(data);

    // A counter that counts the number of bytes read. Is used to determine when
    // to stop this algorithm.
    size_t bytes_read = 0;

    // Flag used to exit the block read for-loop when the transfer has
    // completed.
    bool finished = false;

    for (size_t block_offset = 0; !finished; block_offset++)
    {
      Block_t block = ReadBlock((block_address + block_offset) * kBlockSize);

      // Start the position of the index pointer at the position
      for (uint32_t index = 0; index < kBlockSize; index++)
      {
        data_ptr[bytes_read++] = block.byte[index];
        if (bytes_read >= size)
        {
          finished = true;
          break;
        }
      }
    }

    return Status::kSuccess;
  }

  /// Returns the SD card's information as a reference.
  /// Used for testing purposes and should not be used in production code.
  /// In production code, the CardInfo_t object returned is a dummy object and
  /// is NOT the one used by the object.
  CardInfo_t & GetCardInfo()
  {
    if constexpr (build::IsPlatform(build::Platform::host))
    {
      return sd_;
    }
    else
    {
      static CardInfo_t dummy_sd_card_info;
      return dummy_sd_card_info;
    }
  }

 private:
  struct Block_t
  {
    sjsu::Status status = sjsu::Status::kSuccess;
    // +2 for CRC
    std::array<uint8_t, kBlockSize + 2> byte = { 0 };

    void SetCrcBytes()
    {
      uint16_t crc = GetCrc16(byte.data(), kBlockSize);
      // Set the last byte to the LSB of the crc
      byte.end()[-1] = static_cast<uint8_t>(bit::Extract(crc, 0, 8));
      // Set the second to last byte to the MSB of the crc
      byte.end()[-2] = static_cast<uint8_t>(bit::Extract(crc, 8, 8));
    }
  };

  // Returns string to represent a boolean value
  const char * ToBool(bool condition)
  {
    return (condition) ? "true" : "false";
  }

  bool CommandWasAcknowledged(Response_t & response)
  {
    if (response.byte[0] == 0x00 || response.byte[0] == 0x01)
    {
      return true;
    }

    LogDebug("Parameter Err: %s", ToBool(response.byte[0] & 0x40));
    LogDebug("Addr Err: %s", ToBool(response.byte[0] & 0x20));
    LogDebug("Erase Seq Err: %s", ToBool(response.byte[0] & 0x10));
    LogDebug("Com CRC Err: %s", ToBool(response.byte[0] & 0x08));
    LogDebug("Illegal Cmd Err: %s", ToBool(response.byte[0] & 0x04));
    LogDebug("Erase Reset: %s", ToBool(response.byte[0] & 0x02));
    LogDebug("In Idle: %s", ToBool(response.byte[0] & 0x01));

    return true;
  }

  // Read any number of blocks from the SD card
  CsdBuffer_t GetCsdRegisterBlock()
  {
    CsdBuffer_t csd;

    // Wait for a previous command to finish
    WaitWhileBusy();

    Response_t response = SendCommand(Command::kGetCsd, 0, KeepAlive::kYes);

    // Check if the command was acknowledged properly
    if (!CommandWasAcknowledged(response))
    {
      LogDebug("Get CSD Register was not acknowledged properly!");
      csd.status = sjsu::Status::kBusError;
      return csd;
    }

    // Wait for the card to respond with a ready signal
    WaitToReadBlock();

    // Read all the bytes of a single csd
    for (uint16_t i = 0; i < CsdBuffer_t::kBytesInCsdRegister; i++)
    {
      csd.byte[i] = static_cast<uint8_t>(spi_.Transfer(0xFF));
    }

    // Then read the last two bytes to get the 16-bit CRC
    uint32_t actual_crc = csd.byte.end()[-2] << 8 | csd.byte.end()[-1];
    uint32_t expected_crc =
        GetCrc16(csd.byte.data(), CsdBuffer_t::kBytesInCsdRegister - 2);

    if (expected_crc != actual_crc)
    {
      LogError("CRC mismatch! Expected '0x%04X' :: Got '0x%04X'", expected_crc,
               actual_crc);
      csd.status = sjsu::Status::kBusError;
      return csd;
    }

    // Wait for the SD card to go out of idle state
    do
    {
      response = SendCommand(Command::kGetStatus, 32, KeepAlive::kNo);
    } while (response.byte[0] & 0x01);

    return csd;
  }

  /// Read any number of blocks from the SD card
  Block_t ReadBlock(uint32_t block_address)
  {
    Block_t block;

    LogDebug("Block %" PRId32, block_address);
    // Wait for a previous command to finish
    WaitWhileBusy();

    Response_t response =
        SendCommand(Command::kReadSingle, block_address, KeepAlive::kYes);

    // Check if the command was acknowledged properly
    if (!CommandWasAcknowledged(response))
    {
      LogDebug("Read Cmd was not acknowledged properly!");
      block.status = sjsu::Status::kBusError;
      return block;
    }

    // Wait for the card to respond with a ready signal
    WaitToReadBlock();

    // Read all the bytes of a single block
    for (uint16_t i = 0; i < block.byte.size(); i++)
    {
      block.byte[i] = static_cast<uint8_t>(spi_.Transfer(0xFF));
    }

    // Then read the last two bytes to get the 16-bit CRC
    uint32_t block_crc = block.byte.end()[-2] << 8 | block.byte.end()[-1];
    uint32_t expected_block_crc = GetCrc16(block.byte.data(), kBlockSize);

    if (expected_block_crc != block_crc)
    {
      LogError("Expected '0x%04X' :: Got '0x%04X'", expected_block_crc,
               block_crc);
      block.status = sjsu::Status::kBusError;
    }

    // Wait for the SD card to go out of idle state
    do
    {
      response = SendCommand(Command::kGetStatus, 32, KeepAlive::kNo);
    } while (response.byte[0] & 0x01);

    return block;
  }

  // Writes any number of 512-byte blocks to the SD Card
  uint8_t WriteBlock(uint32_t address, const Block_t & block)
  {
    // Wait for a previous command to finish
    WaitWhileBusy();

    // Send initial write command
    Response_t response =
        SendCommand(Command::kWriteSingle, address, KeepAlive::kYes);
    sjsu::LogDebug("Sent Write Cmd");
    sjsu::LogDebug("[R1 Response:0x%02X]", response.byte[0]);

    // Check if the response was acknowledged properly
    if (response.byte[0] != 0x00)
    {
      sjsu::LogDebug("Error: Write Cmd was not acknowledged properly!");
      sjsu::LogDebug("Parameter Err: %s", ToBool(response.byte[0] & 0x40));
      sjsu::LogDebug("Addr Err: %s", ToBool(response.byte[0] & 0x20));
      sjsu::LogDebug("Erase Seq Err: %s", ToBool(response.byte[0] & 0x10));
      sjsu::LogDebug("Com CRC Err: %s", ToBool(response.byte[0] & 0x08));
      sjsu::LogDebug("Illegal Cmd Err: %s", ToBool(response.byte[0] & 0x04));
      sjsu::LogDebug("Erase Reset: %s", ToBool(response.byte[0] & 0x02));
      sjsu::LogDebug("In Idle: %s", ToBool(response.byte[0] & 0x01));
      return response.byte[0];
    }

    // Send the start token for the current block
    constexpr uint8_t kWriteStartToken = 0xFE;
    spi_.Transfer(kWriteStartToken);

    // Write all 512-bytes of the given block
    spi_.Transfer(block.byte);

    // Read the data response token after writing the block
    uint8_t data_response_token = static_cast<uint8_t>(spi_.Transfer(0xFF));
    sjsu::LogDebug("Response Byte");
    sjsu::LogDebug("[Data Response Token: 0x%02X]", data_response_token);
    sjsu::LogDebug("Data Accepted?: %s", ToBool(data_response_token & 0x05));
    sjsu::LogDebug("Data Rejected (bad crc)?: %s",
                   ToBool(data_response_token & 0b0000'1000));
    sjsu::LogDebug("Data Rejected (write err)?: %s",
                   ToBool(data_response_token & 0b0001'0000));

    WaitWhileBusy();

    return response.byte[0];
  }

  // Deletes any number of blocks (inclusively) within a range of address.
  Status EraseBlock(uint32_t address, size_t length)
  {
    // Wait for a previous command to finish
    WaitWhileBusy();

    // Set the delete start address
    LogDebug("Setting Delete Start Address...");
    Response_t response =
        SendCommand(Command::kDelFrom, address, KeepAlive::kYes);

    // Wait while the writing the start address
    WaitWhileBusy();

    // Force return if an error occurred
    if (response.byte[0] != 0x00)
    {
      LogError("Failed to set Start Address!");
      return Status::kInvalidParameters;
    }

    // Set the delete end address
    LogDebug("Setting Delete End Address...");
    response = SendCommand(Command::kDelTo, address + length, KeepAlive::kYes);

    // Wait while the writing the end address
    WaitWhileBusy();

    // Force return if an error occurred
    if (response.byte[0] != 0x00)
    {
      LogError("Failed to set End Address!");
      return Status::kInvalidParameters;
    }

    // Issue the delete command to delete from our from:to range
    LogDebug("Issuing Delete Command...");
    response = SendCommand(Command::kDel, 0xFFFFFFFF, KeepAlive::kYes);

    // Wait while the deletion occurs
    WaitWhileBusy();

    // Check response
    LogDebug("[R1 Response: 0x%02X]", response.byte[0]);
    LogDebug("Deletion Complete...");

    return Status::kSuccess;
  }

  // Waits for the card to respond after a single or multi block read cmd is
  // sent.
  void WaitToReadBlock()
  {
    // Since the command encountered no errors, we can now begin to
    // read data. The card will enter "BUSY" mode following reception
    // of the read command and sending of a response soon after; we
    // must wait until either the data token "OxFE" or the error token
    // "b000X_XXXX" is received. The error token's flags have the
    // following meanings:
    // MSB   -->  0 (irrelevant)
    // Bit 6 -->  0 (irrelevant)
    // Bit 5 -->  0 (irrelevant)
    // Bit 4 -->  If set, card is locked
    // Bit 3 -->  If set, address is out or range
    // Bit 2 -->  If set, card ECC failed
    // Bit 1 -->  If set, CC error occurred
    // Bit 0 -->  If set, a generic error occurred
    uint8_t wait_byte = 0x00;
    do
    {
      wait_byte = static_cast<uint8_t>(spi_.Transfer(0xFF));
    } while (wait_byte != 0xFE && (wait_byte & 0xE0) != 0x00);

    // DEBUG: Check the value of the wait byte
    if (wait_byte == 0xFE)
    {
      LogDebug(
          "Received GO Byte 0xFE; SD Card is now sending block payload...");
    }
    else if ((wait_byte & 0xE0) == 0x00)
    {
      LogDebug("Error: SD Card Rejected Read Cmd [Response: 0x%02X]",
               wait_byte);
      LogDebug("Card Locked?: %s", ToBool(wait_byte & 0x10));
      LogDebug("Addr Out of Range?: %s", ToBool(wait_byte & 0x08));
      LogDebug("Card ECC Failed?: %s", ToBool(wait_byte & 0x04));
      LogDebug("CC Error?: %s", ToBool(wait_byte & 0x02));
      LogDebug("Error?: %s", ToBool(wait_byte & 0x01));
    }
  }

  // Waits for the card to be ready to receive a new block after one has
  // been written or erased
  void WaitWhileBusy()
  {
    // Wait for the card to finish programming (i.e. when the
    // bytes return to 0xFF)
    LogDebug("Card is busy. Waiting for it to finish...");
    while (spi_.Transfer(0xFF) != 0xFF)
    {
      continue;
    }
    LogDebug("Card finished!");
  }

  void SendCommandParameters(Command sdc, uint32_t arg)
  {
    // Calculate the 7-bit CRC (i.e. CRC7) using the SD card standard's
    // algorithm
    uint8_t payload[] = { static_cast<uint8_t>(sdc),
                          static_cast<uint8_t>(arg >> 24),
                          static_cast<uint8_t>(arg >> 16),
                          static_cast<uint8_t>(arg >> 8),
                          static_cast<uint8_t>(arg >> 0) };

    uint8_t crc = GetCrc7(payload, sizeof(payload));
    if (sdc == Command::kGarbage)
    {
      crc = 0xFF;
    }

    // Send the desired command frame to the SD card board
    for (uint8_t byte : payload)
    {
      spi_.Transfer(byte);
    }
    // Send 7-bit CRC and LSB stop addr (as b1)
    crc = static_cast<uint8_t>((crc << 1) | 0x01);
    spi_.Transfer(crc);
  }

  uint32_t GetResponseLength(Command sdc)
  {
    ResponseType response_type;

    // Determine the response type of the set command
    switch (sdc)
    {
      case Command::kGarbage: response_type = ResponseType::kR1; break;
      case Command::kReset: response_type = ResponseType::kR1; break;
      case Command::kInit: response_type = ResponseType::kR1; break;
      case Command::kGetOp: response_type = ResponseType::kR7; break;
      case Command::kGetCsd: response_type = ResponseType::kR1; break;
      case Command::kStopTrans: response_type = ResponseType::kR1; break;
      case Command::kGetStatus: response_type = ResponseType::kR2; break;
      case Command::kAcBegin: response_type = ResponseType::kR1; break;
      case Command::kAcInit: response_type = ResponseType::kR1; break;
      case Command::kGetOcr: response_type = ResponseType::kR3; break;
      case Command::kChgBlkLen: response_type = ResponseType::kR1; break;
      case Command::kReadSingle: response_type = ResponseType::kR1; break;
      case Command::kReadMulti: response_type = ResponseType::kR1; break;
      case Command::kWriteSingle: response_type = ResponseType::kR1; break;
      case Command::kWriteMulti: response_type = ResponseType::kR1; break;
      case Command::kDelFrom: response_type = ResponseType::kR1; break;
      case Command::kDelTo: response_type = ResponseType::kR1; break;
      case Command::kDel: response_type = ResponseType::kR1b; break;
      default:
        LogError("Unknown response type. Aborting!");
        return -1;
        break;
    }

    // Determine response length (in bytes) based on response type
    switch (response_type)
    {
      case ResponseType::kR1: return 1; break;
      case ResponseType::kR1b: return 1; break;
      case ResponseType::kR2: return 2; break;
      case ResponseType::kR3: return 5; break;
      case ResponseType::kR7: return 5; break;
      default:
        LogError("Response unsupported in SPI mode. Aborting!");
        return -1;
        break;
    }
  }

  /// Send a command to the SD card
  Response_t SendCommand(Command command,
                         uint32_t parameter,
                         KeepAlive keep_alive)
  {
    // Select the SD Card
    chip_select_.SetLow();

    // Send command to the SD Card
    SendCommandParameters(command, parameter);

    // Need to send at least 1 byte of garbage before checking for a response
    spi_.Transfer(0xFF);

    // Continuously poll SD card and when we get a byte that is not 0xFF, figure
    // out where the bit offset is and record the byte that no longer is 0xFF.
    uint32_t bit_offset    = 0;
    uint64_t response_data = 0;
    // Stores the response byte from the for-loop below for later use.
    uint8_t response_byte = 0;

    for (uint32_t tries = 0; tries < kRetryLimit; tries++)
    {
      response_byte = static_cast<uint8_t>(spi_.Transfer(0xFF));
      if (response_byte != 0xFF)
      {
        // Determine the offset, since the first byte of a
        // response will always be 0.
        while (response_byte & (0x80 >> bit_offset))
        {
          bit_offset++;
        }
        break;
      }
    }

    uint32_t response_length = GetResponseLength(command);
    uint32_t bytes_to_read   = response_length;
    if (bit_offset > 0)
    {
      // Read an extra 8 bits since the response was offset
      bytes_to_read++;
    }

    for (uint32_t i = 0; i < bytes_to_read; i++)
    {
      // Make space for the next byte
      response_data = response_data << 8;
      // Add response byte to response data
      response_data |= response_byte;
      // Collect the next byte in the response
      response_byte = static_cast<uint8_t>(spi_.Transfer(0xFF));
    }

    // Compensate for the bit offset
    response_data = response_data >> bit_offset;

    // Creating the response object to return
    Response_t response_result;
    response_result.length = response_length;

    for (uint32_t i = 0; i < response_length; i++)
    {
      uint64_t res = (response_data >> 8 * (response_length - 1 - i));
      response_result.byte[i] = static_cast<uint8_t>(res);
    }

    // Only end the transaction if keep_alive isn't requested
    if (keep_alive == KeepAlive::kNo)
    {
      // Deselect the SPI comm board
      chip_select_.SetHigh();
    }

    return response_result;
  }

  void ClockCard(int number_of_cycles)
  {
    for (int i = 0; i < number_of_cycles / 8; i++)
    {
      spi_.Transfer(0xFF);
    }
  }

  // Initialize and enable the SD Card
  Status Mount()
  {
    // As found in the "Application Note Secure Digital Card Interface for the
    // MSP430" we need to assert the chip select and clock for more than 74
    // cycles.

    // Lower the clock speed to 400kHz for mounting
    spi_.SetClock(400_kHz);

    // So we assert the chip select
    chip_select_.SetLow();
    // Clock out 10 bytes or 80 clock cycles, to get the SD Card's internal
    // state ready for transmission.
    ClockCard(80);
    // Then de-assert chip select
    chip_select_.SetHigh();
    // And finally clock for 16 more cycles to finish off the necessary time for
    // the SD card to finish its internal work.
    ClockCard(16);

    // Reset the card and force it to go to idle state at <400kHz with a
    // CMD0 + (active-low) CS
    LogDebug("Resetting SD Card...");
    SendCommand(Command::kReset, 0, KeepAlive::kNo);

    // =========================================================================
    // Ask if supported voltage of 3v3 is supported
    // =========================================================================
    Status voltage_set_status = CheckSupportedVoltages();
    if (!IsOk(voltage_set_status))
    {
      return voltage_set_status;
    }

    bool initialization_successful = false;
    for (int i = 0; i < kRetryLimit * 10; i++)
    {
      Response_t begin_response =
          SendCommand(Command::kAcBegin, 0, KeepAlive::kNo);

      if (bit::Read(begin_response.byte[0], kIllegalCommand))
      {
        LogDebug("ACMD Begin is illegal!!");
        break;
      }

      uint32_t init_settings = (0b0101'0000 << 24);

      Response_t init_response =
          SendCommand(Command::kAcInit, init_settings, KeepAlive::kNo);
      // If this bit is a 1, then the init process is still going.
      // If it is 0, then the init process has finished.
      if (!bit::Read(init_response.byte[0], kIdle))
      {
        LogDebug("Initialization Successful (0x%02X)", init_response.byte[0]);
        initialization_successful = true;
        break;
      }
    }

    if (!initialization_successful)
    {
      LogDebug("Initialization Failed!");
      return Status::kTimedOut;
    }

    // =========================================================================
    // Reset the card again to trigger SPI mode
    // =========================================================================
    // bool reset_was_successful = WaitForCardToIdle();
    // if (!reset_was_successful)
    // {
    //   LogError("Failed to initiate SPI mode within timeout. Aborting!");
    //   return Status::kTimedOut;
    // }

    // LogDebug("Check if 3v3 is supported!");
    // Response_t response = SendCommand(Command::kGetOcr, 0, KeepAlive::kNo);
    // if ((response.byte[2] & 0xC0) == 0xC0)
    // {
    //   LogDebug("3v3 is NOT supported!");
    //   return Status::kInvalidSettings;
    // }

    // Status card_initialization_status = InitializeSDCard();
    // if (!IsOk(card_initialization_status))
    // {
    //   LogError("SD Card Initialization timed out. Aborting!");
    //   return Status::kTimedOut;
    // }

    // =========================================================================
    // Get Card Capacity Type
    // =========================================================================
    sd_.type = GetCardType();
    if (sd_.type == Type::kSDSC)
    {
      LogDebug("SD Card is SC! We do not support Standard Size SD cards!");
      return Status::kInvalidSettings;
    }
    LogDebug("SD Card is HC/XC");

    // =========================================================================
    // Get and store CSD Register
    // =========================================================================
    sd_.csd = GetCsdRegisterBlock();

    if (!IsOk(sd_.csd.status))
    {
      return sd_.csd.status;
    }

    // Bring the clock speed back up for typical use
    spi_.SetClock(spi_clock_rate_);

    return Status::kSuccess;
  }

  Status InitializeSDCard()
  {
    Response_t response;
    constexpr uint32_t kBusyBit = 0x1;
    int tries                   = 0;
    for (tries = 0; tries < kRetryLimit && response.byte[0] & kBusyBit; tries++)
    {
      // Send host's operating conditions
      response = SendCommand(Command::kInit, 0x40000000, KeepAlive::kYes);
    }

    if (tries > kRetryLimit)
    {
      LogError("SD Card timed out. Aborting!");
      return Status::kTimedOut;
    }

    return Status::kSuccess;
  }

  /// Send the host's supported voltage (3.3V) and ask if the card supports it.
  Status CheckSupportedVoltages()
  {
    LogDebug("Checking Current SD Card Voltage Level...");
    constexpr bit::Mask kVoltageCodeMask = bit::CreateMaskFromRange(8, 11) >> 8;
    constexpr uint8_t kCheckPattern      = 0xAB;
    uint8_t voltage_code                 = 0x01;
    uint32_t voltage_pattern             = (voltage_code << 8) | kCheckPattern;

    Response_t response =
        SendCommand(Command::kGetOp, voltage_pattern, KeepAlive::kYes);

    LogDebug("Detecting if device is SD version +2.0 or not");
    if (bit::Read(response.byte[0], kIllegalCommand))
    {
      LogDebug(
          "Card responded with Illegal Command, this is not a supported SD "
          "card.");
      return Status::kInvalidSettings;
    }

    if (bit::Extract(response.byte[3], kVoltageCodeMask) != voltage_code)
    {
      // If the 2nd-to-last byte of the reponse AND with our host device's
      // supported voltage range is 0x00, the SD card doesn't support our
      // device's operating voltage
      LogError("Unsupported voltage in use. Aborting! (0x%02X) :: (0x%02X)",
               response.byte[3],
               bit::Extract(response.byte[3], kVoltageCodeMask));
      debug::Hexdump(response.byte.data(), response.byte.size());
      return Status::kInvalidSettings;
    }

    if (response.byte[4] != kCheckPattern)
    {
      // If the last byte is not an exact echo of the LSB of the kGetOp
      // command's argument, this response is invalid
      LogError("Response integrity check failed. Aborting! (0x%02X)",
               response.byte[4]);
      debug::Hexdump(response.byte.data(), response.byte.size());
      return Status::kBusError;
    }

    return Status::kSuccess;
  }

  bool WaitForCardToIdle()
  {
    int tries = 0;
    for (tries = 0; tries < kRetryLimit; tries++)
    {
      Response_t response = SendCommand(Command::kReset, 0, KeepAlive::kYes);

      // Check if R1 response frame's bit 1 is set (to ensure that
      // card is in idle state)
      if (response.byte[0] != 0xFF && (response.byte[0] & 0x01))
      {
        // If it is, we can move on; otherwise, keep trying for a set
        // amount of tries
        break;
      }
      Delay(10ms);
    }

    if (tries > kRetryLimit)
    {
      return false;
    }
    return true;
  }

  Type GetCardType()
  {
    LogDebug("Reading Card Capacity Information...");
    Response_t response = SendCommand(Command::kGetOcr, 0x00, KeepAlive::kYes);

    if (response.byte[1] & 0x40)
    {
      return Type::kSDHC;
    }

    // This is not supported for this driver.
    return Type::kSDSC;
  }

  // Adds a message byte to the current CRC-7 to get a the new CRC-7
  static uint8_t Crc7Add(uint8_t crc, uint8_t message_byte)
  {
    return kCrcTable8.crc_table[(crc << 1) ^ message_byte];
  }

  // Returns the CRC-7 for a message of "length" bytes
  static uint8_t GetCrc7(const uint8_t * message, uint8_t length)
  {
    uint8_t crc = 0;
    for (int i = 0; i < length; i++)
    {
      crc = Crc7Add(crc, message[i]);
    }
    return crc;
  }

  // Returns CCITT CRC-16 for a message of "length" bytes
  static uint16_t GetCrc16(const uint8_t * message, uint16_t length)
  {
    uint64_t crc = 0x0000;
    uint64_t temp;
    uint64_t final_value = 0;

    for (uint64_t count = 0; count < length; ++count)
    {
      temp = (*message++ ^ (crc >> 8)) & 0xff;
      crc  = kCrcTable16.crc_table[temp] ^ (crc << 8);
    }

    return static_cast<uint16_t>(crc ^ final_value);
  }

  Spi & spi_;
  Gpio & chip_select_;
  Gpio & card_detect_;
  Gpio::State card_detect_active_level_;
  units::frequency::hertz_t spi_clock_rate_;
  CardInfo_t sd_;
};  // namespace sjsu::experimental
}  // namespace sjsu::experimental