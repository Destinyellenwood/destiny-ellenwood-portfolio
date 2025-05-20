# Lab 05 – LCD Display Applications (MSP430FR6989)

## Overview
This lab demonstrates the use of the MSP430FR6989's built-in `LCD_C` controller to drive a multi-digit segment LCD. Students implemented basic printing, real-time counters, and an advanced chronometer application using timers, interrupts, and low-power modes.

## Key Concepts
- LCD segment control using memory-mapped registers (`LCDMx`)
- 4-multiplexed LCD configuration with ACLK (32.768 KHz) as the timing source
- Mapping 16-bit integers to digit positions
- Using Timer A for time tracking
- External interrupts for button input handling
- Low-power mode integration with interrupt wake-up

## Subprojects

| Subfolder | Description |
|-----------|-------------|
| [5.1_LCD_Print_Number](./5.1_LCD_Print_Number/) | Prints a 5-digit number (e.g. `12345`) to the LCD display |
| [5.2_LCD_Counter_With_Buttons](./5.2_LCD_Counter_With_Buttons/) | Real-time counter with S1 to reset and S2 to increment by 1000 |
| [5.3_Chronometer_Application](./5.3_Chronometer_Application/) | Advanced stopwatch with pause/resume, fast-forward, rewind, and LCD icons |

## Skills Demonstrated
- Peripheral initialization (LCD_C, Timer A, GPIO)
- Polling vs. interrupt-based input handling
- Timer A with 1-second precision using ACLK
- LCD animation with colon and icons
- Low-power operation with `__low_power_mode_3()`

---

This lab demonstrates a complete pipeline: from low-level LCD configuration to full-featured user interfaces built on top of embedded C.
