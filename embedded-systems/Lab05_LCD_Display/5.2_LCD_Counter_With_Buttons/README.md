# Lab 5.2 – LCD Counter with Button Control (MSP430FR6989)

## Overview
This program displays a running counter on the MSP430's LCD. Timer A generates an interrupt every second, incrementing the counter. Two pushbuttons provide user interaction:
- S1 resets the counter to 0
- S2 adds 1000 to the counter

The LCD is updated in real-time, and the microcontroller enters Low Power Mode 3 (LPM3) between events.

## Objectives
- Use Timer A to track real-time seconds
- Handle GPIO interrupts for button presses
- Display 16-bit values on a 5-digit LCD
- Enter low-power mode to conserve energy

## Behavior
- Counter increments once per second
- S1 sets counter to 0
- S2 adds 1000 to counter (wraps at 65535)
- LCD updates automatically

## Source Files
- `main.c` — Real-time counter with low-power support and LCD display
