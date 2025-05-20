# Lab 6.1 – Basic UART Communication (MSP430FR6989)

## Overview
This program demonstrates simple character-based UART communication on the MSP430FR6989. It transmits digits 0–9 and responds to user input ('1' or '2') to control an LED.

## Objectives
- Set up UART using SMCLK @ 1 MHz
- Transmit ASCII digits
- Read characters and use them to control hardware
- Toggle an LED while looping

## UART Settings
- Baud rate: 9600
- Clock source: SMCLK (1 MHz)
- Oversampling enabled

## Behavior
- MSP430 transmits digits 0–9 repeatedly
- If the PC sends:
  - `'1'` → green LED turns ON
  - `'2'` → green LED turns OFF

## Source Files
- `main.c` — Contains UART configuration, transmission loop, and input parsing
