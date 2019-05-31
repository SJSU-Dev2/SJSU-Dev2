//*****************************************************************************
// LPC407x_8x Microcontroller Startup code for use with LPCXpresso IDE
//
// Version : 150706
//*****************************************************************************
//
// Copyright(C) NXP Semiconductors, 2014-2015
// All rights reserved.
//
// Software that is described herein is for illustrative purposes only
// which provides customers with programming information regarding the
// LPC products.  This software is supplied "AS IS" without any warranties of
// any kind, and NXP Semiconductors and its licensor disclaim any and
// all warranties, express or implied, including all implied warranties of
// merchantability, fitness for a particular purpose and non-infringement of
// intellectual property rights.  NXP Semiconductors assumes no responsibility
// or liability for the use of the software, conveys no license or rights under
// any patent, copyright, mask work right, or any other intellectual property
// rights in or to any products. NXP Semiconductors reserves the right to make
// changes in the software without notification. NXP Semiconductors also makes
// no representation or warranty that such application will be suitable for the
// specified use without further testing or modification.
//
// Permission to use, copy, modify, and distribute this software and its
// documentation is hereby granted, under NXP Semiconductors' and its
// licensor's relevant copyrights in the software, without fee, provided that it
// is used in conjunction with NXP Semiconductors microcontrollers.  This
// copyright, permission, and disclaimer notice must appear in all copies of
// this code.
//*****************************************************************************
#include <FreeRTOS.h>
#include <task.h>

#include <cstdint>
#include <cstring>
#include <iterator>

#include "L0_Platform/lpc40xx/interrupt.hpp"
#include "L0_Platform/ram.hpp"
#include "L0_Platform/startup.hpp"
#include "L1_Peripheral/cortex/fpu.hpp"
#include "L1_Peripheral/cortex/system_timer.hpp"
#include "L1_Peripheral/lpc40xx/system_controller.hpp"
#include "L1_Peripheral/lpc40xx/timer.hpp"
#include "L1_Peripheral/lpc40xx/uart.hpp"
#include "newlib/newlib.hpp"
#include "utility/log.hpp"
#include "utility/macros.hpp"
#include "utility/time.hpp"

// Private namespace to make sure that these do not conflict with other globals
namespace
{
// Create LPC40xx system controller to be used by low level initialization.
sjsu::lpc40xx::SystemController system_controller;
// Create timer0 to be used by lower level initialization for uptime calculation
sjsu::lpc40xx::Timer timer0(sjsu::lpc40xx::Timer::Channel::kTimer0);
// Uart port 0 is used to communicate back to the host computer
sjsu::lpc40xx::Uart uart0(sjsu::lpc40xx::Uart::Port::kUart0);

uint64_t Lpc40xxUptime()
{
  return timer0.GetTimer();
}

int Lpc40xxStdOut(const char * data, size_t length)
{
  uart0.Write(reinterpret_cast<const uint8_t *>(data), length);
  return length;
}

int Lpc40xxStdIn(char * data, size_t length)
{
  uart0.Read(reinterpret_cast<uint8_t *>(data), length);
  return length;
}
}  // namespace

extern "C" uint64_t ThreadRuntimeCounter()
{
  return timer0.GetTimer();
}
// The entry point for the C++ library startup
extern "C"
{
  // NOLINTNEXTLINE(readability-identifier-naming)
  extern void __libc_init_array(void);
  // The entry point for the application.
  // main() is the entry point for newlib based applications
  extern int main();
  void vPortSetupTimerInterrupt(void)  // NOLINT
  {
    // Create system timer to be used by low level initialization.
    sjsu::cortex::SystemTimer system_timer(system_controller);
    // Set the SystemTick frequency to the RTOS tick frequency
    // It is critical that this happens before you set the system_clock, since
    // The system_timer keeps the time that the system_clock uses to delay
    // itself.
    system_timer.SetTickFrequency(config::kRtosFrequency);
    system_timer.SetInterrupt(xPortSysTickHandler);
    sjsu::Status timer_start_status = system_timer.StartTimer();
    SJ2_ASSERT_FATAL(timer_start_status == sjsu::Status::kSuccess,
                     "System Timer (used by FreeRTOS) has FAILED to start!");
  }
}

SJ2_WEAK(void InitializePlatform());

void InitializePlatform()
{
  // Enable FPU (F.loating P.oint U.nit)
  // System will crash if floating point instruction is executed before
  // Initializing the FPU first.
  sjsu::cortex::InitializeFloatingPointUnit();
  // Set Clock Speed
  // SetClockFrequency will timeout return the offset between desire clockspeed
  // and actual clockspeed if the PLL doesn't get a frequency fix within a
  // defined timeout (see L1/system_clock.hpp:kDefaultTimeout)
  while (system_controller.SetClockFrequency(config::kSystemClockRateMhz) != 0)
  {
    // Continually attempt to set the clock frequency to the desired until the
    // delta between desired and actual are 0.
    continue;
  }
  // Enable Peripheral Clock and set its divider to 1 meaning the clock speed
  // fed to all peripherals will be 48Mhz.
  system_controller.SetPeripheralClockDivider(1);
  // Set UART0 baudrate, which is required for printf and scanf to work properly
  uart0.Initialize(config::kBaudRate);
  sjsu::newlib::SetStdout(Lpc40xxStdOut);
  sjsu::newlib::SetStdin(Lpc40xxStdIn);
  // Set timer0 to 1 MHz (1,000,000 Hz) so that the timer increments every 1
  // micro second.
  timer0.Initialize(1'000'000UL);
  sjsu::SetUptimeFunction(Lpc40xxUptime);
}

SJ2_SECTION(".crp") const uint32_t kCrpWord = 0xFFFFFFFF;
// Reset entry point for your code.
// Sets up a simple runtime environment and initializes the C/C++ library.

extern "C" void ResetIsr()
{
  // The Hyperload bootloader takes up stack space to execute. The Hyperload
  // bootloader function launches this ISR manually, but it never returns thus
  // it never cleans up the memory it uses. To get that memory back, we have
  // to manually move the stack pointers back to the top of stack.
  const uint32_t kTopOfStack = reinterpret_cast<intptr_t>(&StackTop);
  sjsu::lpc40xx::__set_PSP(kTopOfStack);
  sjsu::lpc40xx::__set_MSP(kTopOfStack);

  sjsu::SystemInitialize();
  InitializePlatform();
// #pragma ignored "-Wpedantic" to suppress main function call warning
#pragma GCC diagnostic push ignored "-Wpedantic"
  [[maybe_unused]] int32_t result = main();
// Enforce the warning after this point
#pragma GCC diagnostic pop
  // main() shouldn't return, but if it does, we'll just enter an infinite
  // loop
  sjsu::Halt();
}
