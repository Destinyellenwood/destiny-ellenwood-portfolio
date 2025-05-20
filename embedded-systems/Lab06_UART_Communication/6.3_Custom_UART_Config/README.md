# Lab 6.3 – UART with Custom ACLK Configuration (MSP430FR6989)

## Overview
This project configures the UART peripheral (eUSCI_A1) to use ACLK (32.768 KHz crystal) instead of the default SMCLK. Custom modulation settings are applied to experiment with non-default clock sources and baud rate tuning.

## Objectives
- Reconfigure UART to use ACLK
- Set baud rate to ~9600 using 32.768 KHz LFXT
- Send and receive characters using a custom configuration
- Demonstrate deeper control of UART clock behavior

## Behavior
- Reads input from user:
  - `'1'` → green LED ON
  - `'2'` → green LED OFF
- Sends `8457` (encoded as letters `I-E-F-H`)
- Sends: `"This class is challenging but fun!"`
- Toggles red LED with each loop

## UART Settings
- Clock: ACLK (32.768 KHz crystal)
- Baud rate: Approx. 9600
- Oversampling: Disabled
- Modulators: Custom `UCBRSx` bits used

## Source Files
- `main.c` — Full UART reconfiguration and communication demo
