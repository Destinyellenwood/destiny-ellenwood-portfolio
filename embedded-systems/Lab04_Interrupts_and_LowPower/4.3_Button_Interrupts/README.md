# Lab 4.3 – Push Button Interrupts (MSP430FR6989)

## Overview
This experiment introduces external interrupt handling using pushbuttons S1 and S2. The MSP430 detects falling edges on input pins P1.1 and P1.2 to toggle the red and green LEDs.

## Objectives
- Configure Port 1 interrupts
- Use `P1IES`, `P1IFG`, and `P1IE` registers
- Toggle LEDs based on button press

## Key Concepts
- External interrupts (edge-triggered)
- Input debouncing (not yet implemented)
- `PORT1_VECTOR` ISR

## Behavior
- Pressing **S1 (P1.1)** toggles the red LED (P1.0)
- Pressing **S2 (P1.2)** toggles the green LED (P9.7)

## Source Files
- `main.c` — Button-triggered LED toggling using Port 1 interrupts
