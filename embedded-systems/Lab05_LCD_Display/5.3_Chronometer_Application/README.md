# Lab 5.3 – LCD Chronometer Application (MSP430FR6989)

## Overview
This advanced embedded application simulates a digital stopwatch (chronometer) using the LCD, Timer A, and two push buttons. It includes a full real-time clock system, user interaction for fast-forwarding and rewinding, and UI icons such as colon blinking and exclamation alert.

## Features
- HH:MM:SS display format using LCD_C
- Run/Stop toggle (S1)
- Long press S1 → Reset time
- Hold S2:
  - with S1 → Fast forward
  - without S1 → Rewind
- LCD colon blinks while running
- LCD shows “TMR” and “!” symbols for user status

## Key Concepts
- LCD symbol memory (`LCDMx`)
- Long-press detection
- Dual button interaction
- Timer-driven timekeeping with LPM3
- Clean UI feedback via LCD segments

## Source Files
- `main.c` – Chronometer code with all runtime features
