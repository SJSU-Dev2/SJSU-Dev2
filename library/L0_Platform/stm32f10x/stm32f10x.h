#pragma once
/**
 ******************************************************************************
 * @file    stm32f10x.h
 * @author  MCD Application Team
 * @version V3.5.0
 * @date    11-March-2011
 * @brief   CMSIS Cortex-M3 Device Peripheral Access Layer Header File.
 *          This file contains all the peripheral register's definitions, bits
 *          definitions and memory mapping for STM32F10x Connectivity line,
 *          High density, High density value line, Medium density,
 *          Medium density Value line, Low density, Low density Value line
 *          and XL-density devices.
 *
 *          The file is the unique include file that the application programmer
 *          is using in the C source code, usually in main.c. This file
 *contains:
 *           - Configuration section that allows to select:
 *              - The device used in the target application
 *              - To use or not the peripheral’s drivers in application
 *code(i.e. code will be based on direct access to peripheral’s registers rather
 *than drivers API), this option is controlled by
 *                "#define USE_STDPERIPH_DRIVER"
 *              - To change few application-specific parameters such as the HSE
 *                crystal frequency
 *           - Data structures and the address mapping for all peripherals
 *           - Peripheral's registers declarations and bits definition
 *           - Macros to access peripheral’s registers hardware
 *
 ******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 ******************************************************************************
 */

/** @addtogroup CMSIS
 * @{
 */

/** @addtogroup stm32f10x
 * @{
 */

#ifndef __STM32F10x_H
#define __STM32F10x_H

#pragma GCC system_header

#include "L0_Platform/arm_cortex/m4/core_cm4.h"
#include <cstdint>

#if defined(__cplusplus)
// SJSU-Dev2: Putting contents of this include in sjsu::lpc17xx
namespace sjsu::stm32f10x
{
extern "C"
{
#endif

/** @addtogroup Library_configuration_section
 * @{
 */

/* Uncomment the line below according to the target STM32 device used in your
   application


#if !defined(STM32F10X_LD) && !defined(STM32F10X_LD_VL) && \
  !defined(STM32F10X_MD) && !defined(STM32F10X_MD_VL) && \
  !defined(STM32F10X_HD) && !defined(STM32F10X_HD_VL) && \
  !defined(STM32F10X_XL) && !defined(STM32F10X_CL)


/*STM32F10X_LD:     STM32 Low density devices
  STM32F10X_LD_VL:  STM32 Low density Value Line devices
  STM32F10X_MD:     STM32 Medium density devices Ex: stm32f103CB
  STM32F10X_MD_VL:  STM32 Medium density Value Line devices
  STM32F10X_HD:     STM32 High density devices
  STM32F10X_HD_VL:  STM32 High density value line devices
  STM32F10X_XL:     STM32 XL-density devices
  STM32F10X_CL:     STM32 Connectivity line devices */

// #define STM32F10X_LD
// #define STM32F10X_LD_VL
#define STM32F10X_MD
  // #define STM32F10X_MD_VL
  // #define STM32F10X_HD
  // #define STM32F10X_HD_VL
  // #define STM32F10X_XL
  // #define STM32F10X_CL
#endif
  /*  Tip: To avoid modifying this file each time you need to switch between
   these devices, you can define the device in your toolchain compiler
   preprocessor.

   - Low-density devices are STM32F101xx, STM32F102xx and STM32F103xx
   microcontrollers where the Flash memory density ranges between 16 and 32
   Kbytes.
   - Low-density value line devices are STM32F100xx microcontrollers where the
   Flash memory density ranges between 16 and 32 Kbytes.
   - Medium-density devices are STM32F101xx, STM32F102xx and STM32F103xx
   microcontrollers where the Flash memory density ranges between 64 and 128
   Kbytes.
   - Medium-density value line devices are STM32F100xx microcontrollers where
   the Flash memory density ranges between 64 and 128 Kbytes.
   - High-density devices are STM32F101xx and STM32F103xx microcontrollers where
     the Flash memory density ranges between 256 and 512 Kbytes.
   - High-density value line devices are STM32F100xx microcontrollers where the
     Flash memory density ranges between 256 and 512 Kbytes.
   - XL-density devices are STM32F101xx and STM32F103xx microcontrollers where
     the Flash memory density ranges between 512 and 1024 Kbytes.
   - Connectivity line devices are STM32F105xx and STM32F107xx microcontrollers.
    */

#if !defined(STM32F10X_LD) && !defined(STM32F10X_LD_VL) && \
    !defined(STM32F10X_MD) && !defined(STM32F10X_MD_VL) && \
    !defined(STM32F10X_HD) && !defined(STM32F10X_HD_VL) && \
    !defined(STM32F10X_XL) && !defined(STM32F10X_CL)
#error \
    "Please select first the target STM32F10x device used in your application (in stm32f10x.h file)"
#endif

#if !defined USE_STDPERIPH_DRIVER
  /**
   * @brief Comment the line below if you will not use the peripherals drivers.
     In this case, these drivers will not be included and the application code
   will be based on direct access to peripherals registers
     */
  /*#define USE_STDPERIPH_DRIVER*/
#endif

/**
 * @brief In the following line adjust the value of External High Speed
 oscillator (HSE) used in your application
   Tip: To avoid modifying this file each time you need to use different HSE,
 you can define the HSE value in your toolchain compiler preprocessor.
  */
#if !defined HSE_VALUE
#ifdef STM32F10X_CL
#define HSE_VALUE \
  ((uint32_t)25000000) /*!< Value of the External oscillator in Hz */
#else
#define HSE_VALUE \
  ((uint32_t)8000000) /*!< Value of the External oscillator in Hz */
#endif                /* STM32F10X_CL */
#endif                /* HSE_VALUE */

/**
 * @brief In the following line adjust the External High Speed oscillator (HSE)
 Startup Timeout value
   */

/*!< Time out for HSE start up  */
#define HSE_STARTUP_TIMEOUT ((uint16_t)0x0500)

#define HSI_VALUE \
  ((uint32_t)8000000) /*!< Value of the Internal oscillator in Hz*/

/**
 * @brief STM32F10x Standard Peripheral Library version number
 */
#define __STM32F10X_STDPERIPH_VERSION_MAIN (0x03) /*!< [31:24] main version */
#define __STM32F10X_STDPERIPH_VERSION_SUB1 (0x05) /*!< [23:16] sub1 version */
#define __STM32F10X_STDPERIPH_VERSION_SUB2 (0x00) /*!< [15:8]  sub2 version */
#define __STM32F10X_STDPERIPH_VERSION_RC                                       \
  (0x00) /*!< [7:0]  release candidate \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \
          * \                                                                  \
          * \ \                                                                             \
          * \ \ \                                                                             \
          * \ \ \ \                                                                             \
          * \ \ \ \ \                                                                             \
          */
#define __STM32F10X_STDPERIPH_VERSION           \
  ((__STM32F10X_STDPERIPH_VERSION_MAIN << 24) | \
   (__STM32F10X_STDPERIPH_VERSION_SUB1 << 16) | \
   (__STM32F10X_STDPERIPH_VERSION_SUB2 << 8) |  \
   (__STM32F10X_STDPERIPH_VERSION_RC))

/**
 * @}
 */

/** @addtogroup Configuration_section_for_CMSIS
 * @{
 */

/**
 * @brief Configuration of the Cortex-M3 Processor and Core Peripherals
 */
#ifdef STM32F10X_XL
#define __MPU_PRESENT 1 /*!< STM32 XL-density devices provide an MPU */
#else
#define __MPU_PRESENT 0    /*!< Other STM32 devices does not provide an MPU */
#endif                     /* STM32F10X_XL */
#define __NVIC_PRIO_BITS 4 /*!< STM32 uses 4 Bits for the Priority Levels */
#define __Vendor_SysTickConfig \
  0 /*!< Set to 1 if different SysTick Config is used */

  /**
   * @brief STM32F10x Interrupt Number Definition, according to the selected
   * device in @ref Library_configuration_section
   */
  enum IRQn
  {
    /******  Cortex-M3 Processor Exceptions Numbers
     ***************************************************/
    NonMaskableInt_IRQn = -14, /*!< 2 Non Maskable Interrupt */
    MemoryManagement_IRQn =
        -12, /*!< 4 Cortex-M3 Memory Management Interrupt              */
    BusFault_IRQn     = -11, /*!< 5 Cortex-M3 Bus Fault Interrupt */
    UsageFault_IRQn   = -10, /*!< 6 Cortex-M3 Usage Fault Interrupt */
    SVCall_IRQn       = -5,  /*!< 11 Cortex-M3 SV Call Interrupt  */
    DebugMonitor_IRQn = -4,  /*!< 12 Cortex-M3 Debug Monitor Interrupt  */
    PendSV_IRQn       = -2,  /*!< 14 Cortex-M3 Pend SV Interrupt  */
    SysTick_IRQn      = -1,  /*!< 15 Cortex-M3 System Tick Interrupt  */

    /******  STM32 specific Interrupt Numbers
     *********************************************************/
    WWDG_IRQn = 0, /*!< Window WatchDog Interrupt                            */
    PVD_IRQn  = 1, /*!< PVD through EXTI Line detection Interrupt            */
    TAMPER_IRQn = 2, /*!< Tamper Interrupt */
    RTC_IRQn   = 3, /*!< RTC global Interrupt                                 */
    FLASH_IRQn = 4, /*!< FLASH global Interrupt                               */
    RCC_IRQn   = 5, /*!< RCC global Interrupt                                 */
    EXTI0_IRQn = 6, /*!< EXTI Line0 Interrupt                                 */
    EXTI1_IRQn = 7, /*!< EXTI Line1 Interrupt                                 */
    EXTI2_IRQn = 8, /*!< EXTI Line2 Interrupt                                 */
    EXTI3_IRQn = 9, /*!< EXTI Line3 Interrupt                                 */
    EXTI4_IRQn = 10,         /*!< EXTI Line4 Interrupt         */
    DMA1_Channel1_IRQn = 11, /*!< DMA1 Channel 1 global Interrupt */
    DMA1_Channel2_IRQn = 12, /*!< DMA1 Channel 2 global Interrupt */
    DMA1_Channel3_IRQn = 13, /*!< DMA1 Channel 3 global Interrupt */
    DMA1_Channel4_IRQn = 14, /*!< DMA1 Channel 4 global Interrupt */
    DMA1_Channel5_IRQn = 15, /*!< DMA1 Channel 5 global Interrupt */
    DMA1_Channel6_IRQn = 16, /*!< DMA1 Channel 6 global Interrupt */
    DMA1_Channel7_IRQn = 17, /*!< DMA1 Channel 7 global Interrupt */

#ifdef STM32F10X_LD
    ADC1_2_IRQn = 18, /*!< ADC1 and ADC2 global Interrupt */
    USB_HP_CAN1_TX_IRQn =
        19, /*!< USB Device High Priority or CAN1 TX Interrupts       */
    USB_LP_CAN1_RX0_IRQn =
        20, /*!< USB Device Low Priority or CAN1 RX0 Interrupts       */
    CAN1_RX1_IRQn     = 21, /*!< CAN1 RX1 Interrupt */
    CAN1_SCE_IRQn     = 22, /*!< CAN1 SCE Interrupt */
    EXTI9_5_IRQn      = 23, /*!< External Line[9:5] Interrupts */
    TIM1_BRK_IRQn     = 24, /*!< TIM1 Break Interrupt */
    TIM1_UP_IRQn      = 25, /*!< TIM1 Update Interrupt */
    TIM1_TRG_COM_IRQn = 26, /*!< TIM1 Trigger and Commutation Interrupt */
    TIM1_CC_IRQn      = 27, /*!< TIM1 Capture Compare Interrupt */
    TIM2_IRQn = 28, /*!< TIM2 global Interrupt                                */
    TIM3_IRQn = 29, /*!< TIM3 global Interrupt                                */
    I2C1_EV_IRQn = 31, /*!< I2C1 Event Interrupt */
    I2C1_ER_IRQn = 32, /*!< I2C1 Error Interrupt */
    SPI1_IRQn = 35, /*!< SPI1 global Interrupt                                */
    USART1_IRQn    = 37, /*!< USART1 global Interrupt */
    USART2_IRQn    = 38, /*!< USART2 global Interrupt */
    EXTI15_10_IRQn = 40, /*!< External Line[15:10] Interrupts */
    RTCAlarm_IRQn  = 41, /*!< RTC Alarm through EXTI Line Interrupt */
    USBWakeUp_IRQn =
        42 /*!< USB Device WakeUp from suspend through EXTI Line Interrupt */
#endif     /* STM32F10X_LD */

#ifdef STM32F10X_LD_VL
    ADC1_IRQn = 18, /*!< ADC1 global Interrupt                                */
    EXTI9_5_IRQn        = 23, /*!< External Line[9:5] Interrupts */
    TIM1_BRK_TIM15_IRQn = 24, /*!< TIM1 Break and TIM15 Interrupts */
    TIM1_UP_TIM16_IRQn  = 25, /*!< TIM1 Update and TIM16 Interrupts */
    TIM1_TRG_COM_TIM17_IRQn =
        26, /*!< TIM1 Trigger and Commutation and TIM17 Interrupt     */
    TIM1_CC_IRQn = 27, /*!< TIM1 Capture Compare Interrupt */
    TIM2_IRQn = 28, /*!< TIM2 global Interrupt                                */
    TIM3_IRQn = 29, /*!< TIM3 global Interrupt                                */
    I2C1_EV_IRQn = 31, /*!< I2C1 Event Interrupt */
    I2C1_ER_IRQn = 32, /*!< I2C1 Error Interrupt */
    SPI1_IRQn = 35, /*!< SPI1 global Interrupt                                */
    USART1_IRQn    = 37, /*!< USART1 global Interrupt */
    USART2_IRQn    = 38, /*!< USART2 global Interrupt */
    EXTI15_10_IRQn = 40, /*!< External Line[15:10] Interrupts */
    RTCAlarm_IRQn  = 41, /*!< RTC Alarm through EXTI Line Interrupt */
    CEC_IRQn = 42, /*!< HDMI-CEC Interrupt                                   */
    TIM6_DAC_IRQn = 54, /*!< TIM6 and DAC underrun Interrupt */
    TIM7_IRQn = 55 /*!< TIM7 Interrupt                                       */
#endif             /* STM32F10X_LD_VL */

#ifdef STM32F10X_MD
    ADC1_2_IRQn = 18, /*!< ADC1 and ADC2 global Interrupt */
    USB_HP_CAN1_TX_IRQn =
        19, /*!< USB Device High Priority or CAN1 TX Interrupts       */
    USB_LP_CAN1_RX0_IRQn =
        20, /*!< USB Device Low Priority or CAN1 RX0 Interrupts       */
    CAN1_RX1_IRQn     = 21, /*!< CAN1 RX1 Interrupt */
    CAN1_SCE_IRQn     = 22, /*!< CAN1 SCE Interrupt */
    EXTI9_5_IRQn      = 23, /*!< External Line[9:5] Interrupts */
    TIM1_BRK_IRQn     = 24, /*!< TIM1 Break Interrupt */
    TIM1_UP_IRQn      = 25, /*!< TIM1 Update Interrupt */
    TIM1_TRG_COM_IRQn = 26, /*!< TIM1 Trigger and Commutation Interrupt */
    TIM1_CC_IRQn      = 27, /*!< TIM1 Capture Compare Interrupt */
    TIM2_IRQn = 28, /*!< TIM2 global Interrupt                                */
    TIM3_IRQn = 29, /*!< TIM3 global Interrupt                                */
    TIM4_IRQn = 30, /*!< TIM4 global Interrupt                                */
    I2C1_EV_IRQn = 31, /*!< I2C1 Event Interrupt */
    I2C1_ER_IRQn = 32, /*!< I2C1 Error Interrupt */
    I2C2_EV_IRQn = 33, /*!< I2C2 Event Interrupt */
    I2C2_ER_IRQn = 34, /*!< I2C2 Error Interrupt */
    SPI1_IRQn = 35, /*!< SPI1 global Interrupt                                */
    SPI2_IRQn = 36, /*!< SPI2 global Interrupt                                */
    USART1_IRQn    = 37, /*!< USART1 global Interrupt */
    USART2_IRQn    = 38, /*!< USART2 global Interrupt */
    USART3_IRQn    = 39, /*!< USART3 global Interrupt */
    EXTI15_10_IRQn = 40, /*!< External Line[15:10] Interrupts */
    RTCAlarm_IRQn  = 41, /*!< RTC Alarm through EXTI Line Interrupt */
    USBWakeUp_IRQn =
        42 /*!< USB Device WakeUp from suspend through EXTI Line Interrupt */
#endif     /* STM32F10X_MD */

#ifdef STM32F10X_MD_VL
    ADC1_IRQn = 18, /*!< ADC1 global Interrupt                                */
    EXTI9_5_IRQn        = 23, /*!< External Line[9:5] Interrupts */
    TIM1_BRK_TIM15_IRQn = 24, /*!< TIM1 Break and TIM15 Interrupts */
    TIM1_UP_TIM16_IRQn  = 25, /*!< TIM1 Update and TIM16 Interrupts */
    TIM1_TRG_COM_TIM17_IRQn =
        26, /*!< TIM1 Trigger and Commutation and TIM17 Interrupt     */
    TIM1_CC_IRQn = 27, /*!< TIM1 Capture Compare Interrupt */
    TIM2_IRQn = 28, /*!< TIM2 global Interrupt                                */
    TIM3_IRQn = 29, /*!< TIM3 global Interrupt                                */
    TIM4_IRQn = 30, /*!< TIM4 global Interrupt                                */
    I2C1_EV_IRQn = 31, /*!< I2C1 Event Interrupt */
    I2C1_ER_IRQn = 32, /*!< I2C1 Error Interrupt */
    I2C2_EV_IRQn = 33, /*!< I2C2 Event Interrupt */
    I2C2_ER_IRQn = 34, /*!< I2C2 Error Interrupt */
    SPI1_IRQn = 35, /*!< SPI1 global Interrupt                                */
    SPI2_IRQn = 36, /*!< SPI2 global Interrupt                                */
    USART1_IRQn    = 37, /*!< USART1 global Interrupt */
    USART2_IRQn    = 38, /*!< USART2 global Interrupt */
    USART3_IRQn    = 39, /*!< USART3 global Interrupt */
    EXTI15_10_IRQn = 40, /*!< External Line[15:10] Interrupts */
    RTCAlarm_IRQn  = 41, /*!< RTC Alarm through EXTI Line Interrupt */
    CEC_IRQn = 42, /*!< HDMI-CEC Interrupt                                   */
    TIM6_DAC_IRQn = 54, /*!< TIM6 and DAC underrun Interrupt */
    TIM7_IRQn = 55 /*!< TIM7 Interrupt                                       */
#endif             /* STM32F10X_MD_VL */

#ifdef STM32F10X_HD
    ADC1_2_IRQn = 18, /*!< ADC1 and ADC2 global Interrupt */
    USB_HP_CAN1_TX_IRQn =
        19, /*!< USB Device High Priority or CAN1 TX Interrupts       */
    USB_LP_CAN1_RX0_IRQn =
        20, /*!< USB Device Low Priority or CAN1 RX0 Interrupts       */
    CAN1_RX1_IRQn     = 21, /*!< CAN1 RX1 Interrupt */
    CAN1_SCE_IRQn     = 22, /*!< CAN1 SCE Interrupt */
    EXTI9_5_IRQn      = 23, /*!< External Line[9:5] Interrupts */
    TIM1_BRK_IRQn     = 24, /*!< TIM1 Break Interrupt */
    TIM1_UP_IRQn      = 25, /*!< TIM1 Update Interrupt */
    TIM1_TRG_COM_IRQn = 26, /*!< TIM1 Trigger and Commutation Interrupt */
    TIM1_CC_IRQn      = 27, /*!< TIM1 Capture Compare Interrupt */
    TIM2_IRQn = 28, /*!< TIM2 global Interrupt                                */
    TIM3_IRQn = 29, /*!< TIM3 global Interrupt                                */
    TIM4_IRQn = 30, /*!< TIM4 global Interrupt                                */
    I2C1_EV_IRQn = 31, /*!< I2C1 Event Interrupt */
    I2C1_ER_IRQn = 32, /*!< I2C1 Error Interrupt */
    I2C2_EV_IRQn = 33, /*!< I2C2 Event Interrupt */
    I2C2_ER_IRQn = 34, /*!< I2C2 Error Interrupt */
    SPI1_IRQn = 35, /*!< SPI1 global Interrupt                                */
    SPI2_IRQn = 36, /*!< SPI2 global Interrupt                                */
    USART1_IRQn    = 37, /*!< USART1 global Interrupt */
    USART2_IRQn    = 38, /*!< USART2 global Interrupt */
    USART3_IRQn    = 39, /*!< USART3 global Interrupt */
    EXTI15_10_IRQn = 40, /*!< External Line[15:10] Interrupts */
    RTCAlarm_IRQn  = 41, /*!< RTC Alarm through EXTI Line Interrupt */
    USBWakeUp_IRQn =
        42, /*!< USB Device WakeUp from suspend through EXTI Line Interrupt */
    TIM8_BRK_IRQn     = 43, /*!< TIM8 Break Interrupt */
    TIM8_UP_IRQn      = 44, /*!< TIM8 Update Interrupt */
    TIM8_TRG_COM_IRQn = 45, /*!< TIM8 Trigger and Commutation Interrupt */
    TIM8_CC_IRQn      = 46, /*!< TIM8 Capture Compare Interrupt */
    ADC3_IRQn = 47, /*!< ADC3 global Interrupt                                */
    FSMC_IRQn = 48, /*!< FSMC global Interrupt                                */
    SDIO_IRQn = 49, /*!< SDIO global Interrupt                                */
    TIM5_IRQn = 50, /*!< TIM5 global Interrupt                                */
    SPI3_IRQn = 51, /*!< SPI3 global Interrupt                                */
    UART4_IRQn = 52, /*!< UART4 global Interrupt */
    UART5_IRQn = 53, /*!< UART5 global Interrupt */
    TIM6_IRQn = 54, /*!< TIM6 global Interrupt                                */
    TIM7_IRQn = 55, /*!< TIM7 global Interrupt                                */
    DMA2_Channel1_IRQn = 56, /*!< DMA2 Channel 1 global Interrupt */
    DMA2_Channel2_IRQn = 57, /*!< DMA2 Channel 2 global Interrupt */
    DMA2_Channel3_IRQn = 58, /*!< DMA2 Channel 3 global Interrupt */
    DMA2_Channel4_5_IRQn =
        59 /*!< DMA2 Channel 4 and Channel 5 global Interrupt        */
#endif     /* STM32F10X_HD */

#ifdef STM32F10X_HD_VL
    ADC1_IRQn = 18, /*!< ADC1 global Interrupt                                */
    EXTI9_5_IRQn        = 23, /*!< External Line[9:5] Interrupts */
    TIM1_BRK_TIM15_IRQn = 24, /*!< TIM1 Break and TIM15 Interrupts */
    TIM1_UP_TIM16_IRQn  = 25, /*!< TIM1 Update and TIM16 Interrupts */
    TIM1_TRG_COM_TIM17_IRQn =
        26, /*!< TIM1 Trigger and Commutation and TIM17 Interrupt     */
    TIM1_CC_IRQn = 27, /*!< TIM1 Capture Compare Interrupt */
    TIM2_IRQn = 28, /*!< TIM2 global Interrupt                                */
    TIM3_IRQn = 29, /*!< TIM3 global Interrupt                                */
    TIM4_IRQn = 30, /*!< TIM4 global Interrupt                                */
    I2C1_EV_IRQn = 31, /*!< I2C1 Event Interrupt */
    I2C1_ER_IRQn = 32, /*!< I2C1 Error Interrupt */
    I2C2_EV_IRQn = 33, /*!< I2C2 Event Interrupt */
    I2C2_ER_IRQn = 34, /*!< I2C2 Error Interrupt */
    SPI1_IRQn = 35, /*!< SPI1 global Interrupt                                */
    SPI2_IRQn = 36, /*!< SPI2 global Interrupt                                */
    USART1_IRQn    = 37, /*!< USART1 global Interrupt */
    USART2_IRQn    = 38, /*!< USART2 global Interrupt */
    USART3_IRQn    = 39, /*!< USART3 global Interrupt */
    EXTI15_10_IRQn = 40, /*!< External Line[15:10] Interrupts */
    RTCAlarm_IRQn  = 41, /*!< RTC Alarm through EXTI Line Interrupt */
    CEC_IRQn = 42, /*!< HDMI-CEC Interrupt                                   */
    TIM12_IRQn = 43, /*!< TIM12 global Interrupt */
    TIM13_IRQn = 44, /*!< TIM13 global Interrupt */
    TIM14_IRQn = 45, /*!< TIM14 global Interrupt */
    TIM5_IRQn = 50, /*!< TIM5 global Interrupt                                */
    SPI3_IRQn = 51, /*!< SPI3 global Interrupt                                */
    UART4_IRQn    = 52, /*!< UART4 global Interrupt */
    UART5_IRQn    = 53, /*!< UART5 global Interrupt */
    TIM6_DAC_IRQn = 54, /*!< TIM6 and DAC underrun Interrupt */
    TIM7_IRQn = 55, /*!< TIM7 Interrupt                                       */
    DMA2_Channel1_IRQn = 56, /*!< DMA2 Channel 1 global Interrupt */
    DMA2_Channel2_IRQn = 57, /*!< DMA2 Channel 2 global Interrupt */
    DMA2_Channel3_IRQn = 58, /*!< DMA2 Channel 3 global Interrupt */
    DMA2_Channel4_5_IRQn =
        59, /*!< DMA2 Channel 4 and Channel 5 global Interrupt        */
    DMA2_Channel5_IRQn = 60 /*!< DMA2 Channel 5 global Interrupt (DMA2 Channel 5
                               is mapped at position 60 only if the MISC_REMAP
                               bit in the AFIO_MAPR2 register is set) */
#endif                      /* STM32F10X_HD_VL */

#ifdef STM32F10X_XL
    ADC1_2_IRQn = 18, /*!< ADC1 and ADC2 global Interrupt */
    USB_HP_CAN1_TX_IRQn =
        19, /*!< USB Device High Priority or CAN1 TX Interrupts       */
    USB_LP_CAN1_RX0_IRQn =
        20, /*!< USB Device Low Priority or CAN1 RX0 Interrupts       */
    CAN1_RX1_IRQn = 21, /*!< CAN1 RX1 Interrupt */
    CAN1_SCE_IRQn = 22, /*!< CAN1 SCE Interrupt */
    EXTI9_5_IRQn  = 23, /*!< External Line[9:5] Interrupts */
    TIM1_BRK_TIM9_IRQn =
        24, /*!< TIM1 Break Interrupt and TIM9 global Interrupt       */
    TIM1_UP_TIM10_IRQn =
        25, /*!< TIM1 Update Interrupt and TIM10 global Interrupt     */
    TIM1_TRG_COM_TIM11_IRQn = 26, /*!< TIM1 Trigger and Commutation Interrupt
                                     and TIM11 global interrupt */
    TIM1_CC_IRQn = 27, /*!< TIM1 Capture Compare Interrupt            */
    TIM2_IRQn = 28, /*!< TIM2 global Interrupt                                */
    TIM3_IRQn = 29, /*!< TIM3 global Interrupt                                */
    TIM4_IRQn = 30, /*!< TIM4 global Interrupt                                */
    I2C1_EV_IRQn = 31, /*!< I2C1 Event Interrupt */
    I2C1_ER_IRQn = 32, /*!< I2C1 Error Interrupt */
    I2C2_EV_IRQn = 33, /*!< I2C2 Event Interrupt */
    I2C2_ER_IRQn = 34, /*!< I2C2 Error Interrupt */
    SPI1_IRQn = 35, /*!< SPI1 global Interrupt                                */
    SPI2_IRQn = 36, /*!< SPI2 global Interrupt                                */
    USART1_IRQn    = 37, /*!< USART1 global Interrupt */
    USART2_IRQn    = 38, /*!< USART2 global Interrupt */
    USART3_IRQn    = 39, /*!< USART3 global Interrupt */
    EXTI15_10_IRQn = 40, /*!< External Line[15:10] Interrupts */
    RTCAlarm_IRQn  = 41, /*!< RTC Alarm through EXTI Line Interrupt */
    USBWakeUp_IRQn =
        42, /*!< USB Device WakeUp from suspend through EXTI Line Interrupt */
    TIM8_BRK_TIM12_IRQn =
        43, /*!< TIM8 Break Interrupt and TIM12 global Interrupt      */
    TIM8_UP_TIM13_IRQn =
        44, /*!< TIM8 Update Interrupt and TIM13 global Interrupt     */
    TIM8_TRG_COM_TIM14_IRQn = 45, /*!< TIM8 Trigger and Commutation Interrupt
                                     and TIM14 global interrupt */
    TIM8_CC_IRQn = 46, /*!< TIM8 Capture Compare Interrupt            */
    ADC3_IRQn = 47, /*!< ADC3 global Interrupt                                */
    FSMC_IRQn = 48, /*!< FSMC global Interrupt                                */
    SDIO_IRQn = 49, /*!< SDIO global Interrupt                                */
    TIM5_IRQn = 50, /*!< TIM5 global Interrupt                                */
    SPI3_IRQn = 51, /*!< SPI3 global Interrupt                                */
    UART4_IRQn = 52, /*!< UART4 global Interrupt */
    UART5_IRQn = 53, /*!< UART5 global Interrupt */
    TIM6_IRQn = 54, /*!< TIM6 global Interrupt                                */
    TIM7_IRQn = 55, /*!< TIM7 global Interrupt                                */
    DMA2_Channel1_IRQn = 56, /*!< DMA2 Channel 1 global Interrupt */
    DMA2_Channel2_IRQn = 57, /*!< DMA2 Channel 2 global Interrupt */
    DMA2_Channel3_IRQn = 58, /*!< DMA2 Channel 3 global Interrupt */
    DMA2_Channel4_5_IRQn =
        59 /*!< DMA2 Channel 4 and Channel 5 global Interrupt        */
#endif     /* STM32F10X_XL */

#ifdef STM32F10X_CL
    ADC1_2_IRQn   = 18, /*!< ADC1 and ADC2 global Interrupt  */
    CAN1_TX_IRQn  = 19, /*!< USB Device High Priority or CAN1 TX Interrupts  */
    CAN1_RX0_IRQn = 20, /*!< USB Device Low Priority or CAN1 RX0 Interrupts */
    CAN1_RX1_IRQn = 21, /*!< CAN1 RX1 Interrupt */
    CAN1_SCE_IRQn = 22, /*!< CAN1 SCE Interrupt */
    EXTI9_5_IRQn  = 23, /*!< External Line[9:5] Interrupts */
    TIM1_BRK_IRQn = 24, /*!< TIM1 Break Interrupt */
    TIM1_UP_IRQn  = 25, /*!< TIM1 Update Interrupt */
    TIM1_TRG_COM_IRQn = 26, /*!< TIM1 Trigger and Commutation Interrupt */
    TIM1_CC_IRQn      = 27, /*!< TIM1 Capture Compare Interrupt */
    TIM2_IRQn = 28, /*!< TIM2 global Interrupt                                */
    TIM3_IRQn = 29, /*!< TIM3 global Interrupt                                */
    TIM4_IRQn = 30, /*!< TIM4 global Interrupt                                */
    I2C1_EV_IRQn = 31, /*!< I2C1 Event Interrupt */
    I2C1_ER_IRQn = 32, /*!< I2C1 Error Interrupt */
    I2C2_EV_IRQn = 33, /*!< I2C2 Event Interrupt */
    I2C2_ER_IRQn = 34, /*!< I2C2 Error Interrupt */
    SPI1_IRQn = 35, /*!< SPI1 global Interrupt                                */
    SPI2_IRQn = 36, /*!< SPI2 global Interrupt                                */
    USART1_IRQn    = 37, /*!< USART1 global Interrupt */
    USART2_IRQn    = 38, /*!< USART2 global Interrupt */
    USART3_IRQn    = 39, /*!< USART3 global Interrupt */
    EXTI15_10_IRQn = 40, /*!< External Line[15:10] Interrupts */
    RTCAlarm_IRQn  = 41, /*!< RTC Alarm through EXTI Line Interrupt */
    OTG_FS_WKUP_IRQn =
        42, /*!< USB OTG FS WakeUp from suspend through EXTI Line Interrupt */
    TIM5_IRQn = 50, /*!< TIM5 global Interrupt                                */
    SPI3_IRQn = 51, /*!< SPI3 global Interrupt                                */
    UART4_IRQn = 52, /*!< UART4 global Interrupt */
    UART5_IRQn = 53, /*!< UART5 global Interrupt */
    TIM6_IRQn = 54, /*!< TIM6 global Interrupt                                */
    TIM7_IRQn = 55, /*!< TIM7 global Interrupt                                */
    DMA2_Channel1_IRQn = 56, /*!< DMA2 Channel 1 global Interrupt */
    DMA2_Channel2_IRQn = 57, /*!< DMA2 Channel 2 global Interrupt */
    DMA2_Channel3_IRQn = 58, /*!< DMA2 Channel 3 global Interrupt */
    DMA2_Channel4_IRQn = 59, /*!< DMA2 Channel 4 global Interrupt */
    DMA2_Channel5_IRQn = 60, /*!< DMA2 Channel 5 global Interrupt */
    ETH_IRQn = 61, /*!< Ethernet global Interrupt                            */
    ETH_WKUP_IRQn = 62, /*!< Ethernet Wakeup through EXTI line Interrupt */
    CAN2_TX_IRQn  = 63, /*!< CAN2 TX Interrupt */
    CAN2_RX0_IRQn = 64, /*!< CAN2 RX0 Interrupt */
    CAN2_RX1_IRQn = 65, /*!< CAN2 RX1 Interrupt */
    CAN2_SCE_IRQn = 66, /*!< CAN2 SCE Interrupt */
    OTG_FS_IRQn   = 67  /*!< USB OTG FS global Interrupt  */
#endif                  /* STM32F10X_CL */
  } IRQn_Type;

  /**
   * @}
   */

  /** @addtogroup Exported_types
   * @{
   */

  /*!< STM32F10x Standard Peripheral Library old types (maintained for legacy
   * purpose) */
  typedef int32_t s32;
  typedef int16_t s16;
  typedef int8_t s8;

  typedef const int32_t sc32; /*!< Read Only */
  typedef const int16_t sc16; /*!< Read Only */
  typedef const int8_t sc8;   /*!< Read Only */

  typedef __IO int32_t vs32;
  typedef __IO int16_t vs16;
  typedef __IO int8_t vs8;

  typedef __I int32_t vsc32; /*!< Read Only */
  typedef __I int16_t vsc16; /*!< Read Only */
  typedef __I int8_t vsc8;   /*!< Read Only */

  typedef uint32_t u32;
  typedef uint16_t u16;
  typedef uint8_t u8;

  typedef const uint32_t uc32; /*!< Read Only */
  typedef const uint16_t uc16; /*!< Read Only */
  typedef const uint8_t uc8;   /*!< Read Only */

  typedef __IO uint32_t vu32;
  typedef __IO uint16_t vu16;
  typedef __IO uint8_t vu8;

  typedef __I uint32_t vuc32; /*!< Read Only */
  typedef __I uint16_t vuc16; /*!< Read Only */
  typedef __I uint8_t vuc8;   /*!< Read Only */

  typedef enum
  {
    RESET = 0,
    SET   = !RESET
  } FlagStatus,
      ITStatus;

  typedef enum
  {
    DISABLE = 0,
    ENABLE  = !DISABLE
  } FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

  typedef enum
  {
    ERROR   = 0,
    SUCCESS = !ERROR
  } ErrorStatus;

/*!< STM32F10x Standard Peripheral Library old definitions (maintained for
 * legacy purpose) */
#define HSEStartUp_TimeOut HSE_STARTUP_TIMEOUT
#define HSE_Value HSE_VALUE
#define HSI_Value HSI_VALUE
  /**
   * @}
   */

  /** @addtogroup Peripheral_registers_structures
   * @{
   */

  /**
   * @brief Analog to Digital Converter
   */

  typedef struct
  {
    __IO uint32_t SR;
    __IO uint32_t CR1;
    __IO uint32_t CR2;
    __IO uint32_t SMPR1;
    __IO uint32_t SMPR2;
    __IO uint32_t JOFR1;
    __IO uint32_t JOFR2;
    __IO uint32_t JOFR3;
    __IO uint32_t JOFR4;
    __IO uint32_t HTR;
    __IO uint32_t LTR;
    __IO uint32_t SQR1;
    __IO uint32_t SQR2;
    __IO uint32_t SQR3;
    __IO uint32_t JSQR;
    __IO uint32_t JDR1;
    __IO uint32_t JDR2;
    __IO uint32_t JDR3;
    __IO uint32_t JDR4;
    __IO uint32_t DR;
  } ADC_TypeDef;

  /**
   * @brief Backup Registers
   */

  typedef struct
  {
    uint32_t RESERVED0;
    __IO uint16_t DR1;
    uint16_t RESERVED1;
    __IO uint16_t DR2;
    uint16_t RESERVED2;
    __IO uint16_t DR3;
    uint16_t RESERVED3;
    __IO uint16_t DR4;
    uint16_t RESERVED4;
    __IO uint16_t DR5;
    uint16_t RESERVED5;
    __IO uint16_t DR6;
    uint16_t RESERVED6;
    __IO uint16_t DR7;
    uint16_t RESERVED7;
    __IO uint16_t DR8;
    uint16_t RESERVED8;
    __IO uint16_t DR9;
    uint16_t RESERVED9;
    __IO uint16_t DR10;
    uint16_t RESERVED10;
    __IO uint16_t RTCCR;
    uint16_t RESERVED11;
    __IO uint16_t CR;
    uint16_t RESERVED12;
    __IO uint16_t CSR;
    uint16_t RESERVED13[5];
    __IO uint16_t DR11;
    uint16_t RESERVED14;
    __IO uint16_t DR12;
    uint16_t RESERVED15;
    __IO uint16_t DR13;
    uint16_t RESERVED16;
    __IO uint16_t DR14;
    uint16_t RESERVED17;
    __IO uint16_t DR15;
    uint16_t RESERVED18;
    __IO uint16_t DR16;
    uint16_t RESERVED19;
    __IO uint16_t DR17;
    uint16_t RESERVED20;
    __IO uint16_t DR18;
    uint16_t RESERVED21;
    __IO uint16_t DR19;
    uint16_t RESERVED22;
    __IO uint16_t DR20;
    uint16_t RESERVED23;
    __IO uint16_t DR21;
    uint16_t RESERVED24;
    __IO uint16_t DR22;
    uint16_t RESERVED25;
    __IO uint16_t DR23;
    uint16_t RESERVED26;
    __IO uint16_t DR24;
    uint16_t RESERVED27;
    __IO uint16_t DR25;
    uint16_t RESERVED28;
    __IO uint16_t DR26;
    uint16_t RESERVED29;
    __IO uint16_t DR27;
    uint16_t RESERVED30;
    __IO uint16_t DR28;
    uint16_t RESERVED31;
    __IO uint16_t DR29;
    uint16_t RESERVED32;
    __IO uint16_t DR30;
    uint16_t RESERVED33;
    __IO uint16_t DR31;
    uint16_t RESERVED34;
    __IO uint16_t DR32;
    uint16_t RESERVED35;
    __IO uint16_t DR33;
    uint16_t RESERVED36;
    __IO uint16_t DR34;
    uint16_t RESERVED37;
    __IO uint16_t DR35;
    uint16_t RESERVED38;
    __IO uint16_t DR36;
    uint16_t RESERVED39;
    __IO uint16_t DR37;
    uint16_t RESERVED40;
    __IO uint16_t DR38;
    uint16_t RESERVED41;
    __IO uint16_t DR39;
    uint16_t RESERVED42;
    __IO uint16_t DR40;
    uint16_t RESERVED43;
    __IO uint16_t DR41;
    uint16_t RESERVED44;
    __IO uint16_t DR42;
    uint16_t RESERVED45;
  } BKP_TypeDef;

  /**
   * @brief Controller Area Network TxMailBox
   */

  typedef struct
  {
    __IO uint32_t TIR;
    __IO uint32_t TDTR;
    __IO uint32_t TDLR;
    __IO uint32_t TDHR;
  } CAN_TxMailBox_TypeDef;

  /**
   * @brief Controller Area Network FIFOMailBox
   */

  typedef struct
  {
    __IO uint32_t RIR;
    __IO uint32_t RDTR;
    __IO uint32_t RDLR;
    __IO uint32_t RDHR;
  } CAN_FIFOMailBox_TypeDef;

  /**
   * @brief Controller Area Network FilterRegister
   */

  typedef struct
  {
    __IO uint32_t FR1;
    __IO uint32_t FR2;
  } CAN_FilterRegister_TypeDef;

  /**
   * @brief Controller Area Network
   */

  typedef struct
  {
    __IO uint32_t MCR;
    __IO uint32_t MSR;
    __IO uint32_t TSR;
    __IO uint32_t RF0R;
    __IO uint32_t RF1R;
    __IO uint32_t IER;
    __IO uint32_t ESR;
    __IO uint32_t BTR;
    uint32_t RESERVED0[88];
    CAN_TxMailBox_TypeDef sTxMailBox[3];
    CAN_FIFOMailBox_TypeDef sFIFOMailBox[2];
    uint32_t RESERVED1[12];
    __IO uint32_t FMR;
    __IO uint32_t FM1R;
    uint32_t RESERVED2;
    __IO uint32_t FS1R;
    uint32_t RESERVED3;
    __IO uint32_t FFA1R;
    uint32_t RESERVED4;
    __IO uint32_t FA1R;
    uint32_t RESERVED5[8];
#ifndef STM32F10X_CL
    CAN_FilterRegister_TypeDef sFilterRegister[14];
#else
  CAN_FilterRegister_TypeDef sFilterRegister[28];
#endif /* STM32F10X_CL */
  } CAN_TypeDef;

  /**
   * @brief Consumer Electronics Control (CEC)
   */
  typedef struct
  {
    __IO uint32_t CFGR;
    __IO uint32_t OAR;
    __IO uint32_t PRES;
    __IO uint32_t ESR;
    __IO uint32_t CSR;
    __IO uint32_t TXD;
    __IO uint32_t RXD;
  } CEC_TypeDef;

  /**
   * @brief CRC calculation unit
   */

  typedef struct
  {
    __IO uint32_t DR;
    __IO uint8_t IDR;
    uint8_t RESERVED0;
    uint16_t RESERVED1;
    __IO uint32_t CR;
  } CRC_TypeDef;

  /**
   * @brief Digital to Analog Converter
   */

  typedef struct
  {
    __IO uint32_t CR;
    __IO uint32_t SWTRIGR;
    __IO uint32_t DHR12R1;
    __IO uint32_t DHR12L1;
    __IO uint32_t DHR8R1;
    __IO uint32_t DHR12R2;
    __IO uint32_t DHR12L2;
    __IO uint32_t DHR8R2;
    __IO uint32_t DHR12RD;
    __IO uint32_t DHR12LD;
    __IO uint32_t DHR8RD;
    __IO uint32_t DOR1;
    __IO uint32_t DOR2;
#if defined(STM32F10X_LD_VL) || defined(STM32F10X_MD_VL) || \
    defined(STM32F10X_HD_VL)
    __IO uint32_t SR;
#endif
  } DAC_TypeDef;

  /**
   * @brief Debug MCU
   */

  typedef struct
  {
    __IO uint32_t IDCODE;
    __IO uint32_t CR;
  } DBGMCU_TypeDef;

  /**
   * @brief DMA Controller
   */

  typedef struct
  {
    __IO uint32_t CCR;
    __IO uint32_t CNDTR;
    __IO uint32_t CPAR;
    __IO uint32_t CMAR;
  } DMA_Channel_TypeDef;

  typedef struct
  {
    __IO uint32_t ISR;
    __IO uint32_t IFCR;
  } DMA_TypeDef;

  /**
   * @brief Ethernet MAC
   */

  typedef struct
  {
    __IO uint32_t MACCR;
    __IO uint32_t MACFFR;
    __IO uint32_t MACHTHR;
    __IO uint32_t MACHTLR;
    __IO uint32_t MACMIIAR;
    __IO uint32_t MACMIIDR;
    __IO uint32_t MACFCR;
    __IO uint32_t MACVLANTR; /*    8 */
    uint32_t RESERVED0[2];
    __IO uint32_t MACRWUFFR; /*   11 */
    __IO uint32_t MACPMTCSR;
    uint32_t RESERVED1[2];
    __IO uint32_t MACSR; /*   15 */
    __IO uint32_t MACIMR;
    __IO uint32_t MACA0HR;
    __IO uint32_t MACA0LR;
    __IO uint32_t MACA1HR;
    __IO uint32_t MACA1LR;
    __IO uint32_t MACA2HR;
    __IO uint32_t MACA2LR;
    __IO uint32_t MACA3HR;
    __IO uint32_t MACA3LR; /*   24 */
    uint32_t RESERVED2[40];
    __IO uint32_t MMCCR; /*   65 */
    __IO uint32_t MMCRIR;
    __IO uint32_t MMCTIR;
    __IO uint32_t MMCRIMR;
    __IO uint32_t MMCTIMR; /*   69 */
    uint32_t RESERVED3[14];
    __IO uint32_t MMCTGFSCCR; /*   84 */
    __IO uint32_t MMCTGFMSCCR;
    uint32_t RESERVED4[5];
    __IO uint32_t MMCTGFCR;
    uint32_t RESERVED5[10];
    __IO uint32_t MMCRFCECR;
    __IO uint32_t MMCRFAECR;
    uint32_t RESERVED6[10];
    __IO uint32_t MMCRGUFCR;
    uint32_t RESERVED7[334];
    __IO uint32_t PTPTSCR;
    __IO uint32_t PTPSSIR;
    __IO uint32_t PTPTSHR;
    __IO uint32_t PTPTSLR;
    __IO uint32_t PTPTSHUR;
    __IO uint32_t PTPTSLUR;
    __IO uint32_t PTPTSAR;
    __IO uint32_t PTPTTHR;
    __IO uint32_t PTPTTLR;
    uint32_t RESERVED8[567];
    __IO uint32_t DMABMR;
    __IO uint32_t DMATPDR;
    __IO uint32_t DMARPDR;
    __IO uint32_t DMARDLAR;
    __IO uint32_t DMATDLAR;
    __IO uint32_t DMASR;
    __IO uint32_t DMAOMR;
    __IO uint32_t DMAIER;
    __IO uint32_t DMAMFBOCR;
    uint32_t RESERVED9[9];
    __IO uint32_t DMACHTDR;
    __IO uint32_t DMACHRDR;
    __IO uint32_t DMACHTBAR;
    __IO uint32_t DMACHRBAR;
  } ETH_TypeDef;

  /**
   * @brief External Interrupt/Event Controller
   */

  typedef struct
  {
    __IO uint32_t IMR;
    __IO uint32_t EMR;
    __IO uint32_t RTSR;
    __IO uint32_t FTSR;
    __IO uint32_t SWIER;
    __IO uint32_t PR;
  } EXTI_TypeDef;

  /**
   * @brief FLASH Registers
   */

  typedef struct
  {
    __IO uint32_t ACR;
    __IO uint32_t KEYR;
    __IO uint32_t OPTKEYR;
    __IO uint32_t SR;
    __IO uint32_t CR;
    __IO uint32_t AR;
    __IO uint32_t RESERVED;
    __IO uint32_t OBR;
    __IO uint32_t WRPR;
#ifdef STM32F10X_XL
    uint32_t RESERVED1[8];
    __IO uint32_t KEYR2;
    uint32_t RESERVED2;
    __IO uint32_t SR2;
    __IO uint32_t CR2;
    __IO uint32_t AR2;
#endif /* STM32F10X_XL */
  } FLASH_TypeDef;

  /**
   * @brief Option Bytes Registers
   */

  typedef struct
  {
    __IO uint16_t RDP;
    __IO uint16_t USER;
    __IO uint16_t Data0;
    __IO uint16_t Data1;
    __IO uint16_t WRP0;
    __IO uint16_t WRP1;
    __IO uint16_t WRP2;
    __IO uint16_t WRP3;
  } OB_TypeDef;

  /**
   * @brief Flexible Static Memory Controller
   */

  typedef struct
  {
    __IO uint32_t BTCR[8];
  } FSMC_Bank1_TypeDef;

  /**
   * @brief Flexible Static Memory Controller Bank1E
   */

  typedef struct
  {
    __IO uint32_t BWTR[7];
  } FSMC_Bank1E_TypeDef;

  /**
   * @brief Flexible Static Memory Controller Bank2
   */

  typedef struct
  {
    __IO uint32_t PCR2;
    __IO uint32_t SR2;
    __IO uint32_t PMEM2;
    __IO uint32_t PATT2;
    uint32_t RESERVED0;
    __IO uint32_t ECCR2;
  } FSMC_Bank2_TypeDef;

  /**
   * @brief Flexible Static Memory Controller Bank3
   */

  typedef struct
  {
    __IO uint32_t PCR3;
    __IO uint32_t SR3;
    __IO uint32_t PMEM3;
    __IO uint32_t PATT3;
    uint32_t RESERVED0;
    __IO uint32_t ECCR3;
  } FSMC_Bank3_TypeDef;

  /**
   * @brief Flexible Static Memory Controller Bank4
   */

  typedef struct
  {
    __IO uint32_t PCR4;
    __IO uint32_t SR4;
    __IO uint32_t PMEM4;
    __IO uint32_t PATT4;
    __IO uint32_t PIO4;
  } FSMC_Bank4_TypeDef;

  /**
   * @brief General Purpose I/O
   */

  typedef struct
  {
    __IO uint32_t CRL;
    __IO uint32_t CRH;
    __IO uint32_t IDR;
    __IO uint32_t ODR;
    __IO uint32_t BSRR;
    __IO uint32_t BRR;
    __IO uint32_t LCKR;
  } GPIO_TypeDef;

  /**
   * @brief Alternate Function I/O
   */

  typedef struct
  {
    __IO uint32_t EVCR;
    __IO uint32_t MAPR;
    __IO uint32_t EXTICR[4];
    uint32_t RESERVED0;
    __IO uint32_t MAPR2;
  } AFIO_TypeDef;
  /**
   * @brief Inter Integrated Circuit Interface
   */

  typedef struct
  {
    __IO uint16_t CR1;
    uint16_t RESERVED0;
    __IO uint16_t CR2;
    uint16_t RESERVED1;
    __IO uint16_t OAR1;
    uint16_t RESERVED2;
    __IO uint16_t OAR2;
    uint16_t RESERVED3;
    __IO uint16_t DR;
    uint16_t RESERVED4;
    __IO uint16_t SR1;
    uint16_t RESERVED5;
    __IO uint16_t SR2;
    uint16_t RESERVED6;
    __IO uint16_t CCR;
    uint16_t RESERVED7;
    __IO uint16_t TRISE;
    uint16_t RESERVED8;
  } I2C_TypeDef;

  /**
   * @brief Independent WATCHDOG
   */

  typedef struct
  {
    __IO uint32_t KR;
    __IO uint32_t PR;
    __IO uint32_t RLR;
    __IO uint32_t SR;
  } IWDG_TypeDef;

  /**
   * @brief Power Control
   */

  typedef struct
  {
    __IO uint32_t CR;
    __IO uint32_t CSR;
  } PWR_TypeDef;

  /**
   * @brief Reset and Clock Control
   */

  typedef struct
  {
    __IO uint32_t CR;
    __IO uint32_t CFGR;
    __IO uint32_t CIR;
    __IO uint32_t APB2RSTR;
    __IO uint32_t APB1RSTR;
    __IO uint32_t AHBENR;
    __IO uint32_t APB2ENR;
    __IO uint32_t APB1ENR;
    __IO uint32_t BDCR;
    __IO uint32_t CSR;

#ifdef STM32F10X_CL
    __IO uint32_t AHBRSTR;
    __IO uint32_t CFGR2;
#endif /* STM32F10X_CL */

#if defined(STM32F10X_LD_VL) || defined(STM32F10X_MD_VL) || \
    defined(STM32F10X_HD_VL)
    uint32_t RESERVED0;
    __IO uint32_t CFGR2;
#endif /* STM32F10X_LD_VL || STM32F10X_MD_VL || STM32F10X_HD_VL */
  } RCC_TypeDef;

  /**
   * @brief Real-Time Clock
   */

  typedef struct
  {
    __IO uint16_t CRH;
    uint16_t RESERVED0;
    __IO uint16_t CRL;
    uint16_t RESERVED1;
    __IO uint16_t PRLH;
    uint16_t RESERVED2;
    __IO uint16_t PRLL;
    uint16_t RESERVED3;
    __IO uint16_t DIVH;
    uint16_t RESERVED4;
    __IO uint16_t DIVL;
    uint16_t RESERVED5;
    __IO uint16_t CNTH;
    uint16_t RESERVED6;
    __IO uint16_t CNTL;
    uint16_t RESERVED7;
    __IO uint16_t ALRH;
    uint16_t RESERVED8;
    __IO uint16_t ALRL;
    uint16_t RESERVED9;
  } RTC_TypeDef;

  /**
   * @brief SD host Interface
   */

  typedef struct
  {
    __IO uint32_t POWER;
    __IO uint32_t CLKCR;
    __IO uint32_t ARG;
    __IO uint32_t CMD;
    __I uint32_t RESPCMD;
    __I uint32_t RESP1;
    __I uint32_t RESP2;
    __I uint32_t RESP3;
    __I uint32_t RESP4;
    __IO uint32_t DTIMER;
    __IO uint32_t DLEN;
    __IO uint32_t DCTRL;
    __I uint32_t DCOUNT;
    __I uint32_t STA;
    __IO uint32_t ICR;
    __IO uint32_t MASK;
    uint32_t RESERVED0[2];
    __I uint32_t FIFOCNT;
    uint32_t RESERVED1[13];
    __IO uint32_t FIFO;
  } SDIO_TypeDef;

  /**
   * @brief Serial Peripheral Interface
   */

  typedef struct
  {
    __IO uint16_t CR1;
    uint16_t RESERVED0;
    __IO uint16_t CR2;
    uint16_t RESERVED1;
    __IO uint16_t SR;
    uint16_t RESERVED2;
    __IO uint16_t DR;
    uint16_t RESERVED3;
    __IO uint16_t CRCPR;
    uint16_t RESERVED4;
    __IO uint16_t RXCRCR;
    uint16_t RESERVED5;
    __IO uint16_t TXCRCR;
    uint16_t RESERVED6;
    __IO uint16_t I2SCFGR;
    uint16_t RESERVED7;
    __IO uint16_t I2SPR;
    uint16_t RESERVED8;
  } SPI_TypeDef;

  /**
   * @brief TIM
   */

  typedef struct
  {
    __IO uint16_t CR1;
    uint16_t RESERVED0;
    __IO uint16_t CR2;
    uint16_t RESERVED1;
    __IO uint16_t SMCR;
    uint16_t RESERVED2;
    __IO uint16_t DIER;
    uint16_t RESERVED3;
    __IO uint16_t SR;
    uint16_t RESERVED4;
    __IO uint16_t EGR;
    uint16_t RESERVED5;
    __IO uint16_t CCMR1;
    uint16_t RESERVED6;
    __IO uint16_t CCMR2;
    uint16_t RESERVED7;
    __IO uint16_t CCER;
    uint16_t RESERVED8;
    __IO uint16_t CNT;
    uint16_t RESERVED9;
    __IO uint16_t PSC;
    uint16_t RESERVED10;
    __IO uint16_t ARR;
    uint16_t RESERVED11;
    __IO uint16_t RCR;
    uint16_t RESERVED12;
    __IO uint16_t CCR1;
    uint16_t RESERVED13;
    __IO uint16_t CCR2;
    uint16_t RESERVED14;
    __IO uint16_t CCR3;
    uint16_t RESERVED15;
    __IO uint16_t CCR4;
    uint16_t RESERVED16;
    __IO uint16_t BDTR;
    uint16_t RESERVED17;
    __IO uint16_t DCR;
    uint16_t RESERVED18;
    __IO uint16_t DMAR;
    uint16_t RESERVED19;
  } TIM_TypeDef;

  /**
   * @brief Universal Synchronous Asynchronous Receiver Transmitter
   */

  typedef struct
  {
    __IO uint16_t SR;
    uint16_t RESERVED0;
    __IO uint16_t DR;
    uint16_t RESERVED1;
    __IO uint16_t BRR;
    uint16_t RESERVED2;
    __IO uint16_t CR1;
    uint16_t RESERVED3;
    __IO uint16_t CR2;
    uint16_t RESERVED4;
    __IO uint16_t CR3;
    uint16_t RESERVED5;
    __IO uint16_t GTPR;
    uint16_t RESERVED6;
  } USART_TypeDef;

  /**
   * @brief Window WATCHDOG
   */

  typedef struct
  {
    __IO uint32_t CR;
    __IO uint32_t CFR;
    __IO uint32_t SR;
  } WWDG_TypeDef;

  /**
   * @}
   */

  /** @addtogroup Peripheral_memory_map
   * @{
   */

  /*!< FLASH base address in the alias region */
  constexpr intptr_t FLASH_BASE = 0x08000000;
  /*!< SRAM base address in the alias region */
  constexpr intptr_t SRAM_BASE = 0x20000000;
  /*!< Peripheral base address in the alias region */
  constexpr intptr_t PERIPH_BASE = 0x40000000;
  /*!< SRAM base address in the bit-band region */
  constexpr intptr_t SRAM_BB_BASE = 0x22000000;

  /*!< Peripheral base address in the bit-band region */
  constexpr intptr_t PERIPH_BB_BASE = 0x42000000;
  /*!< FSMC registers base address */
  constexpr intptr_t FSMC_R_BASE = 0xA0000000;

  /*!< Peripheral memory map */
  constexpr intptr_t APB1PERIPH_BASE = PERIPH_BASE;
  constexpr intptr_t APB2PERIPH_BASE = (PERIPH_BASE + 0x10000);
  constexpr intptr_t AHBPERIPH_BASE  = (PERIPH_BASE + 0x20000);

  constexpr intptr_t TIM2_BASE   = (APB1PERIPH_BASE + 0x0000);
  constexpr intptr_t TIM3_BASE   = (APB1PERIPH_BASE + 0x0400);
  constexpr intptr_t TIM4_BASE   = (APB1PERIPH_BASE + 0x0800);
  constexpr intptr_t TIM5_BASE   = (APB1PERIPH_BASE + 0x0C00);
  constexpr intptr_t TIM6_BASE   = (APB1PERIPH_BASE + 0x1000);
  constexpr intptr_t TIM7_BASE   = (APB1PERIPH_BASE + 0x1400);
  constexpr intptr_t TIM12_BASE  = (APB1PERIPH_BASE + 0x1800);
  constexpr intptr_t TIM13_BASE  = (APB1PERIPH_BASE + 0x1C00);
  constexpr intptr_t TIM14_BASE  = (APB1PERIPH_BASE + 0x2000);
  constexpr intptr_t RTC_BASE    = (APB1PERIPH_BASE + 0x2800);
  constexpr intptr_t WWDG_BASE   = (APB1PERIPH_BASE + 0x2C00);
  constexpr intptr_t IWDG_BASE   = (APB1PERIPH_BASE + 0x3000);
  constexpr intptr_t SPI2_BASE   = (APB1PERIPH_BASE + 0x3800);
  constexpr intptr_t SPI3_BASE   = (APB1PERIPH_BASE + 0x3C00);
  constexpr intptr_t USART2_BASE = (APB1PERIPH_BASE + 0x4400);
  constexpr intptr_t USART3_BASE = (APB1PERIPH_BASE + 0x4800);
  constexpr intptr_t UART4_BASE  = (APB1PERIPH_BASE + 0x4C00);
  constexpr intptr_t UART5_BASE  = (APB1PERIPH_BASE + 0x5000);
  constexpr intptr_t I2C1_BASE   = (APB1PERIPH_BASE + 0x5400);
  constexpr intptr_t I2C2_BASE   = (APB1PERIPH_BASE + 0x5800);
  constexpr intptr_t CAN1_BASE   = (APB1PERIPH_BASE + 0x6400);
  constexpr intptr_t CAN2_BASE   = (APB1PERIPH_BASE + 0x6800);
  constexpr intptr_t BKP_BASE    = (APB1PERIPH_BASE + 0x6C00);
  constexpr intptr_t PWR_BASE    = (APB1PERIPH_BASE + 0x7000);
  constexpr intptr_t DAC_BASE    = (APB1PERIPH_BASE + 0x7400);
  constexpr intptr_t CEC_BASE    = (APB1PERIPH_BASE + 0x7800);

  constexpr intptr_t AFIO_BASE   = (APB2PERIPH_BASE + 0x0000);
  constexpr intptr_t EXTI_BASE   = (APB2PERIPH_BASE + 0x0400);
  constexpr intptr_t GPIOA_BASE  = (APB2PERIPH_BASE + 0x0800);
  constexpr intptr_t GPIOB_BASE  = (APB2PERIPH_BASE + 0x0C00);
  constexpr intptr_t GPIOC_BASE  = (APB2PERIPH_BASE + 0x1000);
  constexpr intptr_t GPIOD_BASE  = (APB2PERIPH_BASE + 0x1400);
  constexpr intptr_t GPIOE_BASE  = (APB2PERIPH_BASE + 0x1800);
  constexpr intptr_t GPIOF_BASE  = (APB2PERIPH_BASE + 0x1C00);
  constexpr intptr_t GPIOG_BASE  = (APB2PERIPH_BASE + 0x2000);
  constexpr intptr_t ADC1_BASE   = (APB2PERIPH_BASE + 0x2400);
  constexpr intptr_t ADC2_BASE   = (APB2PERIPH_BASE + 0x2800);
  constexpr intptr_t TIM1_BASE   = (APB2PERIPH_BASE + 0x2C00);
  constexpr intptr_t SPI1_BASE   = (APB2PERIPH_BASE + 0x3000);
  constexpr intptr_t TIM8_BASE   = (APB2PERIPH_BASE + 0x3400);
  constexpr intptr_t USART1_BASE = (APB2PERIPH_BASE + 0x3800);
  constexpr intptr_t ADC3_BASE   = (APB2PERIPH_BASE + 0x3C00);
  constexpr intptr_t TIM15_BASE  = (APB2PERIPH_BASE + 0x4000);
  constexpr intptr_t TIM16_BASE  = (APB2PERIPH_BASE + 0x4400);
  constexpr intptr_t TIM17_BASE  = (APB2PERIPH_BASE + 0x4800);
  constexpr intptr_t TIM9_BASE   = (APB2PERIPH_BASE + 0x4C00);
  constexpr intptr_t TIM10_BASE  = (APB2PERIPH_BASE + 0x5000);
  constexpr intptr_t TIM11_BASE  = (APB2PERIPH_BASE + 0x5400);

  constexpr intptr_t SDIO_BASE = (PERIPH_BASE + 0x18000);

  constexpr intptr_t DMA1_BASE          = (AHBPERIPH_BASE + 0x0000);
  constexpr intptr_t DMA1_Channel1_BASE = (AHBPERIPH_BASE + 0x0008);
  constexpr intptr_t DMA1_Channel2_BASE = (AHBPERIPH_BASE + 0x001C);
  constexpr intptr_t DMA1_Channel3_BASE = (AHBPERIPH_BASE + 0x0030);
  constexpr intptr_t DMA1_Channel4_BASE = (AHBPERIPH_BASE + 0x0044);
  constexpr intptr_t DMA1_Channel5_BASE = (AHBPERIPH_BASE + 0x0058);
  constexpr intptr_t DMA1_Channel6_BASE = (AHBPERIPH_BASE + 0x006C);
  constexpr intptr_t DMA1_Channel7_BASE = (AHBPERIPH_BASE + 0x0080);
  constexpr intptr_t DMA2_BASE          = (AHBPERIPH_BASE + 0x0400);
  constexpr intptr_t DMA2_Channel1_BASE = (AHBPERIPH_BASE + 0x0408);
  constexpr intptr_t DMA2_Channel2_BASE = (AHBPERIPH_BASE + 0x041C);
  constexpr intptr_t DMA2_Channel3_BASE = (AHBPERIPH_BASE + 0x0430);
  constexpr intptr_t DMA2_Channel4_BASE = (AHBPERIPH_BASE + 0x0444);
  constexpr intptr_t DMA2_Channel5_BASE = (AHBPERIPH_BASE + 0x0458);
  constexpr intptr_t RCC_BASE           = (AHBPERIPH_BASE + 0x1000);
  constexpr intptr_t CRC_BASE           = (AHBPERIPH_BASE + 0x3000);

  /*!< Flash registers base address */
  constexpr intptr_t FLASH_R_BASE = (AHBPERIPH_BASE + 0x2000);
  /*!< Flash Option Bytes base address */
  constexpr intptr_t OB_BASE = ((uint32_t)0x1FFFF800);

  constexpr intptr_t ETH_BASE     = (AHBPERIPH_BASE + 0x8000);
  constexpr intptr_t ETH_MAC_BASE = (ETH_BASE);
  constexpr intptr_t ETH_MMC_BASE = (ETH_BASE + 0x0100);
  constexpr intptr_t ETH_PTP_BASE = (ETH_BASE + 0x0700);
  constexpr intptr_t ETH_DMA_BASE = (ETH_BASE + 0x1000);

  /*!< FSMC Bank1 registers base address */
  constexpr intptr_t FSMC_Bank1_R_BASE = (FSMC_R_BASE + 0x0000);
  /*!< FSMC Bank1E registers base address */
  constexpr intptr_t FSMC_Bank1E_R_BASE = (FSMC_R_BASE + 0x0104);
  /*!< FSMC Bank2 registers base address */
  constexpr intptr_t FSMC_Bank2_R_BASE = (FSMC_R_BASE + 0x0060);
  /*!< FSMC Bank3 registers base address */
  constexpr intptr_t FSMC_Bank3_R_BASE = (FSMC_R_BASE + 0x0080);
  /*!< FSMC Bank4 registers base address */
  constexpr intptr_t FSMC_Bank4_R_BASE = (FSMC_R_BASE + 0x00A0);
  /*!< Debug MCU registers base address */
  constexpr intptr_t DBGMCU_BASE = 0xE0042000;

  /**
   * @}
   */

  /** @addtogroup Peripheral_declaration
   * @{
   */

  inline auto * const TIM2   = reinterpret_cast<TIM_TypeDef *>(TIM2_BASE);
  inline auto * const TIM3   = reinterpret_cast<TIM_TypeDef *>(TIM3_BASE);
  inline auto * const TIM4   = reinterpret_cast<TIM_TypeDef *>(TIM4_BASE);
  inline auto * const TIM5   = reinterpret_cast<TIM_TypeDef *>(TIM5_BASE);
  inline auto * const TIM6   = reinterpret_cast<TIM_TypeDef *>(TIM6_BASE);
  inline auto * const TIM7   = reinterpret_cast<TIM_TypeDef *>(TIM7_BASE);
  inline auto * const TIM12  = reinterpret_cast<TIM_TypeDef *>(TIM12_BASE);
  inline auto * const TIM13  = reinterpret_cast<TIM_TypeDef *>(TIM13_BASE);
  inline auto * const TIM14  = reinterpret_cast<TIM_TypeDef *>(TIM14_BASE);
  inline auto * const RTC    = reinterpret_cast<RTC_TypeDef *>(RTC_BASE);
  inline auto * const WWDG   = reinterpret_cast<WWDG_TypeDef *>(WWDG_BASE);
  inline auto * const IWDG   = reinterpret_cast<IWDG_TypeDef *>(IWDG_BASE);
  inline auto * const SPI2   = reinterpret_cast<SPI_TypeDef *>(SPI2_BASE);
  inline auto * const SPI3   = reinterpret_cast<SPI_TypeDef *>(SPI3_BASE);
  inline auto * const USART2 = reinterpret_cast<USART_TypeDef *>(USART2_BASE);
  inline auto * const USART3 = reinterpret_cast<USART_TypeDef *>(USART3_BASE);
  inline auto * const UART4  = reinterpret_cast<USART_TypeDef *>(UART4_BASE);
  inline auto * const UART5  = reinterpret_cast<USART_TypeDef *>(UART5_BASE);
  inline auto * const I2C1   = reinterpret_cast<I2C_TypeDef *>(I2C1_BASE);
  inline auto * const I2C2   = reinterpret_cast<I2C_TypeDef *>(I2C2_BASE);
  inline auto * const CAN1   = reinterpret_cast<CAN_TypeDef *>(CAN1_BASE);
  inline auto * const CAN2   = reinterpret_cast<CAN_TypeDef *>(CAN2_BASE);
  inline auto * const BKP    = reinterpret_cast<BKP_TypeDef *>(BKP_BASE);
  inline auto * const PWR    = reinterpret_cast<PWR_TypeDef *>(PWR_BASE);
  inline auto * const DAC    = reinterpret_cast<DAC_TypeDef *>(DAC_BASE);
  inline auto * const CEC    = reinterpret_cast<CEC_TypeDef *>(CEC_BASE);
  inline auto * const AFIO   = reinterpret_cast<AFIO_TypeDef *>(AFIO_BASE);
  inline auto * const EXTI   = reinterpret_cast<EXTI_TypeDef *>(EXTI_BASE);
  inline auto * const GPIOA  = reinterpret_cast<GPIO_TypeDef *>(GPIOA_BASE);
  inline auto * const GPIOB  = reinterpret_cast<GPIO_TypeDef *>(GPIOB_BASE);
  inline auto * const GPIOC  = reinterpret_cast<GPIO_TypeDef *>(GPIOC_BASE);
  inline auto * const GPIOD  = reinterpret_cast<GPIO_TypeDef *>(GPIOD_BASE);
  inline auto * const GPIOE  = reinterpret_cast<GPIO_TypeDef *>(GPIOE_BASE);
  inline auto * const GPIOF  = reinterpret_cast<GPIO_TypeDef *>(GPIOF_BASE);
  inline auto * const GPIOG  = reinterpret_cast<GPIO_TypeDef *>(GPIOG_BASE);
  inline auto * const ADC1   = reinterpret_cast<ADC_TypeDef *>(ADC1_BASE);
  inline auto * const ADC2   = reinterpret_cast<ADC_TypeDef *>(ADC2_BASE);
  inline auto * const TIM1   = reinterpret_cast<TIM_TypeDef *>(TIM1_BASE);
  inline auto * const SPI1   = reinterpret_cast<SPI_TypeDef *>(SPI1_BASE);
  inline auto * const TIM8   = reinterpret_cast<TIM_TypeDef *>(TIM8_BASE);
  inline auto * const USART1 = reinterpret_cast<USART_TypeDef *>(USART1_BASE);
  inline auto * const ADC3   = reinterpret_cast<ADC_TypeDef *>(ADC3_BASE);
  inline auto * const TIM15  = reinterpret_cast<TIM_TypeDef *>(TIM15_BASE);
  inline auto * const TIM16  = reinterpret_cast<TIM_TypeDef *>(TIM16_BASE);
  inline auto * const TIM17  = reinterpret_cast<TIM_TypeDef *>(TIM17_BASE);
  inline auto * const TIM9   = reinterpret_cast<TIM_TypeDef *>(TIM9_BASE);
  inline auto * const TIM10  = reinterpret_cast<TIM_TypeDef *>(TIM10_BASE);
  inline auto * const TIM11  = reinterpret_cast<TIM_TypeDef *>(TIM11_BASE);
  inline auto * const SDIO   = reinterpret_cast<SDIO_TypeDef *>(SDIO_BASE);
  inline auto * const DMA1   = reinterpret_cast<DMA_TypeDef *>(DMA1_BASE);
  inline auto * const DMA2   = reinterpret_cast<DMA_TypeDef *>(DMA2_BASE);
  inline auto * const DMA1_Channel1 =
      reinterpret_cast<DMA_Channel_TypeDef *>(DMA1_Channel1_BASE);
  inline auto * const DMA1_Channel2 =
      reinterpret_cast<DMA_Channel_TypeDef *>(DMA1_Channel2_BASE);
  inline auto * const DMA1_Channel3 =
      reinterpret_cast<DMA_Channel_TypeDef *>(DMA1_Channel3_BASE);
  inline auto * const DMA1_Channel4 =
      reinterpret_cast<DMA_Channel_TypeDef *>(DMA1_Channel4_BASE);
  inline auto * const DMA1_Channel5 =
      reinterpret_cast<DMA_Channel_TypeDef *>(DMA1_Channel5_BASE);
  inline auto * const DMA1_Channel6 =
      reinterpret_cast<DMA_Channel_TypeDef *>(DMA1_Channel6_BASE);
  inline auto * const DMA1_Channel7 =
      reinterpret_cast<DMA_Channel_TypeDef *>(DMA1_Channel7_BASE);
  inline auto * const DMA2_Channel1 =
      reinterpret_cast<DMA_Channel_TypeDef *>(DMA2_Channel1_BASE);
  inline auto * const DMA2_Channel2 =
      reinterpret_cast<DMA_Channel_TypeDef *>(DMA2_Channel2_BASE);
  inline auto * const DMA2_Channel3 =
      reinterpret_cast<DMA_Channel_TypeDef *>(DMA2_Channel3_BASE);
  inline auto * const DMA2_Channel4 =
      reinterpret_cast<DMA_Channel_TypeDef *>(DMA2_Channel4_BASE);
  inline auto * const DMA2_Channel5 =
      reinterpret_cast<DMA_Channel_TypeDef *>(DMA2_Channel5_BASE);
  inline auto * const RCC   = reinterpret_cast<RCC_TypeDef *>(RCC_BASE);
  inline auto * const CRC   = reinterpret_cast<CRC_TypeDef *>(CRC_BASE);
  inline auto * const FLASH = reinterpret_cast<FLASH_TypeDef *>(FLASH_R_BASE);
  inline auto * const OB    = reinterpret_cast<OB_TypeDef *>(OB_BASE);
  inline auto * const ETH   = reinterpret_cast<ETH_TypeDef *>(ETH_BASE);
  inline auto * const FSMC_Bank1 =
      reinterpret_cast<FSMC_Bank1_TypeDef *>(FSMC_Bank1_R_BASE);
  inline auto * const FSMC_Bank1E =
      reinterpret_cast<FSMC_Bank1E_TypeDef *>(FSMC_Bank1E_R_BASE);
  inline auto * const FSMC_Bank2 =
      reinterpret_cast<FSMC_Bank2_TypeDef *>(FSMC_Bank2_R_BASE);
  inline auto * const FSMC_Bank3 =
      reinterpret_cast<FSMC_Bank3_TypeDef *>(FSMC_Bank3_R_BASE);
  inline auto * const FSMC_Bank4 =
      reinterpret_cast<FSMC_Bank4_TypeDef *>(FSMC_Bank4_R_BASE);
  inline auto * const DBGMCU = reinterpret_cast<DBGMCU_TypeDef *>(DBGMCU_BASE);

#if defined(__cplusplus)
  // SJSU-Dev2: Putting contents of this include in sjsu::stm32f10x
}  // namespace sjsu::stm32f10x
}  // extern "C"
#endif /* __STM32F10x_H */
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
