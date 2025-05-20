# Lab 02 – Button Polling with GPIO (MSP430FR6989)

## Overview
This lab explores the use of GPIO inputs and polling techniques to read push buttons on the MSP430FR6989. Two onboard LEDs (P1.0 and P9.7) are controlled by two push buttons (P1.1 and P1.2), demonstrating basic logic and control flow using polling-based input reading.

## Objectives
- Use `P1IN` to read button states
- Set up internal pull-up resistors using `P1REN` and `P1OUT`
- Understand active-low input behavior
- Toggle GPIO outputs based on real-time input state

## Hardware
- **Red LED**: P1.0 (output)
- **Green LED**: P9.7 (output)
- **Button S1**: P1.1 (input with pull-up)
- **Button S2**: P1.2 (input with pull-up)

## Behavior
- Pressing **S1** turns on the **red LED**
- Pressing **S2** turns on the **green LED**
- Pressing **both buttons** lights up both LEDs

## Limitations
- Uses polling (not interrupt-driven)
- Not power-efficient for battery operation
- No debounce logic included

## Source Files
- `main.c` — Contains button polling and LED control logic
