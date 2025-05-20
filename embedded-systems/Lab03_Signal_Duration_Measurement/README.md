# Lab 03 – Signal Duration Measurement (MSP430FR6989)

## Overview
This lab applies the Timer A module to measure how long a push button is held and uses that duration to control an LED. It simulates a real-world task like controlling a load based on input signal duration.

## Objectives
- Configure Timer A in continuous and up modes
- Capture signal pulse width using software polling
- Use timer overflow flag (TAIFG) to detect long presses
- Convert timing measurement into LED duration control

## Hardware
- **Red LED**: P1.0 – Lights up for the duration of the press
- **Green LED**: P9.7 – Indicates overflow (pulse too long)
- **Start Button (S1)**: P1.1 – Starts the timer
- **Reset Button (S2)**: P1.2 – Clears overflow indicator

## Behavior
1. Press **S1** to begin timing
2. Release **S1** to stop timer and record duration
3. If press lasted too long (overflow), **green LED** lights up
4. Otherwise, **red LED** turns on for the measured time
5. Press **S2** to clear green LED

## Techniques Used
- Dual timer modes: Continuous (measurement) and Up (output duration)
- Polling for button transitions
- Manual flag handling with `TAIFG`

## Reflection
This lab demonstrates a full timer-based application: capturing input events, calculating duration, and using timers for controlled output — all without interrupts.

## Source Files
- `main.c` – Signal pulse timer using MSP430 Timer A
