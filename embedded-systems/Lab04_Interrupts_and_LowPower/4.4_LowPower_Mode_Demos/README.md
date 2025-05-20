# Lab 4.4 – Low Power Modes (MSP430FR6989)

## Overview
This section explores Low Power Mode 3 (LPM3) and how the MSP430 can respond to interrupts while conserving energy. Each demo wakes from LPM3 on either a timer interrupt or button press.

## Included Programs
- `main_lpm3_timerA_continuous.c`  
  Timer A in continuous mode toggles red LED while CPU sleeps.
  
- `main_lpm3_timerA_upmode.c`  
  Timer A in up mode toggles both red and green LEDs every 1 second.
  
- `main_lpm3_button_interrupts.c`  
  Button S1 toggles red LED, S2 toggles green LED while in LPM3.

## Key Concepts
- `_low_power_mode_3()` halts CPU while keeping ACLK active
- Timer or GPIO interrupts bring CPU out of sleep temporarily
- Saves significant power in embedded/battery-powered systems

## Source Files
- `main_lpm3_timerA_continuous.c`
- `main_lpm3_timerA_upmode.c`
- `main_lpm3_button_interrupts.c`
