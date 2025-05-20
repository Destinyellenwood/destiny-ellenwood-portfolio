# Lab 1: Flashing LEDs – MSP430FR6989

## Overview
This lab uses the MSP430FR6989 to flash two onboard LEDs (red on P1.0 and green on P9.7) in sync. It demonstrates delay loops, GPIO configuration, and low-level embedded C programming.

## Concepts
- GPIO pin configuration
- Delay loops (`for` and `_delay_cycles`)
- Power-on initialization
- LED toggling using `^=`

## Code Features
- Red and Green LEDs toggle every 3 million clock cycles
- Uses `volatile` variables for delay
- Avoids watchdog resets by disabling it on boot

## How to Run
1. Build and upload using Code Composer Studio
2. Observe both LEDs flashing alternately
3. Confirm code works in **normal mode** (reset button responds)

## Learning Reflection
This lab emphasized the importance of proper GPIO setup and how simple delay loops impact power efficiency. It also revealed the difference between debug and normal execution modes on the MSP430.

## Sample Output
The two onboard LEDs blink together with visible pauses between each state.
