# Lab 03 – LED Timing with Timer A (MSP430FR6989)

## Overview
This lab explores Timer A operation modes to blink LEDs using hardware-timed delays. It replaces software delay loops with precise timing using the ACLK crystal oscillator and the Timer A module in both continuous and up modes.

## Objectives
- Configure Timer A in continuous and up modes
- Use `TAIFG` polling to detect timer overflow
- Toggle LEDs based on timer events
- Use a 32.768 KHz external crystal for accurate timekeeping

## Key Concepts
- ACLK configuration with `LFXIN`
- Timer A control register (`TA0CTL`) settings
- Use of `TA0CCR0` in up mode
- Polling the `TAIFG` flag

## Hardware
- **Red LED**: P1.0 (output)
- **Green LED**: P9.7 (output)
- **ACLK Source**: 32.768 KHz crystal

## Behavior
- Toggles red LED every ~2 seconds using Timer A overflow
- Can be modified to blink faster/slower using clock dividers or up mode

## Extra Applications in Report
- **Up mode with TA0CCR0 for 1s, 0.1s, and 0.01s**
- **Application: Pulse width capture and LED feedback using button input and Timer A**

## Source Files
- `main.c` — Uses Timer A to toggle LED every overflow cycle (~2 seconds at 32.768 KHz)
