# Lab 8.2 – ADC Sequenced Reading of Joystick X and Y (MSP430FR6989)

## Overview
This program expands upon the previous ADC example by configuring the ADC12_B module to read both joystick axes in sequence (X = A10, Y = A4). The sampled values are transmitted via UART to a terminal.

---

## Objectives
- Set up ADC to read multiple channels in sequence
- Use ADC12MEM0 and ADC12MEM1 for X and Y results
- Send both values over UART with clear formatting

---

## Behavior
Every second:
- ADC samples A10 and A4 in sequence
- Transmits formatted output:  
  `X = 2017 | Y = 1982`

- Toggles red LED as a heartbeat indicator

---

## Hardware
- **X-Axis (A10)**: P9.2  
- **Y-Axis (A4)**: P8.7  
- **UART TX**: P3.4  
- **UART RX**: P3.5  
- **Red LED**: P1.0

---

## Key Concepts
- `ADC12CONSEQ_1`: sequence-of-channels mode
- `ADC12EOS`: marks end of sequence on MEM1
- Use of polling (`ADC12BUSY`) for conversion status
- Real-time UART feedback loop

---

## Source Files
- `main.c` — Multi-channel ADC with UART display
