# Lab 4.5 – Crawler System with Finite-State Machine (MSP430FR6989)

## Overview
This advanced lab implements a state-driven crawler system that dynamically adjusts LED blink rates using Timer A and button interrupts. It simulates control behavior where each button press transitions through increasing speeds or changes control modes.

## Objectives
- Implement a software finite-state machine
- Change timer frequency based on application state
- Respond to user input via interrupts
- Build scalable embedded logic with state control

## State Diagram
| State                | LED Behavior             | Timer Speed       |
|---------------------|--------------------------|-------------------|
| RG                  | Both LEDs toggle @ 1 Hz  | 32768 cycles      |
| R_PLUS              | Red toggles, green off    | 2 Hz (16384)      |
| R_PLUS_PLUS         | Red toggles faster        | 4 Hz (8192)       |
| R_PLUS_PLUS_PLUS    | Red toggles fastest       | 8 Hz (4096)       |
| G_PLUS              | Green toggles, red off    | 2 Hz (16384)      |
| G_PLUS_PLUS         | Green toggles faster      | 4 Hz (8192)       |
| G_PLUS_PLUS_PLUS    | Green toggles fastest     | 8 Hz (4096)       |

## Button Controls
- **S1 (P1.1)**: Advances the current state forward (red-focused)
- **S2 (P1.2)**: Switches control to green-focused or reverses

## Key Concepts
- Debouncing with `_delay_cycles()`
- Finite-state logic
- Dynamic adjustment of Timer A `CCR0` value

## Source Files
- `main.c` — Full application with state management and interrupt control
