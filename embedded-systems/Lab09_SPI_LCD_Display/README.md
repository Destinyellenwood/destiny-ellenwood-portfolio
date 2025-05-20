# Lab 09 – SPI LCD Display Control (MSP430FR6989)

## Overview
This lab introduces the use of the **Serial Peripheral Interface (SPI)** to control a 128x128 pixel LCD on the Educational BoosterPack using the MSP430FR6989. It demonstrates both low-level SPI setup and high-level display rendering using TI’s `grlib` graphics library. The LCD is updated with both static content (logos, shapes, text) and dynamic counters using screen switching and Timer A interrupts.

---

## Key Concepts
- eUSCI_B0 SPI configuration: master mode, 3-pin SPI, 8 MHz bit clock
- SPI pin setup using `P1SELx` for UCB0CLK and UCB0SIMO
- LCD data/command and chip-select pin initialization
- Pixel-based graphics rendering using `grlib`
- Switching between a static image screen (Snoopy logo) and a dynamic UI screen
- Timer A used to periodically increment a counter displayed on the LCD
- Low-level LCD driver calls and font/font color settings

---

## Behavior
- Two display modes are supported:
  1. **Screen 1**: Snoopy logo (static image)
  2. **Screen 2**: Displays shapes, text, and a counter with an incrementing Timer A

- Press **S1** (P1.1) to toggle between screens
- The counter resets when switching to screen 2
- The red LED (P1.0) toggles each loop to indicate activity

---

## Files
| File | Description |
|------|-------------|
| `main.c` | Full program that toggles between the logo screen and shape/text/counter screen using S1 |
| `HAL_LCD_PortInit()` | Configures SPI pins and additional GPIO lines for display |
| `HAL_LCD_SpiInit()` | Initializes UCB0 for 3-pin SPI with 8 MHz bit clock |
| `Draw_Snoopy_Logo()` | Uses external image to display a static Snoopy image |
| `Draw_Text_And_Shapes_Screen()` | Draws a custom UI with colored shapes and text, plus an incrementing counter |
| `Initialize_TimerA()` | Configures ACLK-based Timer A to update the screen at 100 ms intervals |

---

## Hardware
- **LCD (Crystalfontz128x128)**: Controlled via SPI
- **SPI MOSI**: P1.6 (UCB0SIMO)
- **SPI CLK**: P1.4 (UCB0CLK)
- **DC Pin**: P1.2
- **CS Pin**: P1.3
- **S1 Button**: P1.1 (toggles screens)
- **Red LED**: P1.0 (blinks on each screen loop)
- **Green LED**: P9.7

---

## UART?  
UART is not used in this lab — all output is visual via the LCD.

---

## Learning Objectives
- Master register-level SPI setup
- Combine low-level hardware config with high-level graphical output
- Handle screen logic and state switching in embedded C
- Manage Timer A interrupts to trigger screen refresh

---

This lab demonstrates professional-grade embedded system design — integrating low-level communication protocols, user input, graphics APIs, and multitasking logic all in a real-time system.
