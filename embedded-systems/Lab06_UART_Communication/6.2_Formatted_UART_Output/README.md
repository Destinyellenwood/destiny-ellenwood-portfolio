# Lab 6.2 – Formatted UART Output (MSP430FR6989)

## Overview
This experiment demonstrates how to transmit formatted integers and strings over UART using the MSP430FR6989. Integers are encoded using a custom mapping (e.g., `0` = `'A'`, `1` = `'B'`, ..., `9` = `'J'`). The program also sends complete strings and handles basic user input to toggle LEDs.

## Objectives
- Implement a function to convert 16-bit integers into serial characters
- Send full ASCII strings using `uart_write_string()`
- Interpret received UART input to control GPIO
- Use SMCLK (1 MHz) as UART clock source

## UART Settings
- Baud rate: 9600
- Clock: SMCLK
- Data: 8-bit, no parity, 1 stop bit
- Oversampling enabled

## Behavior
- If the PC sends `'1'` → green LED turns ON
- If the PC sends `'2'` → green LED turns OFF
- Every ~200 ms:
  - Sends `10287` as `ACBHI`
  - Sends `"Hello UART!"` with newline and carriage return

## Source Files
- `main.c` — UART setup, integer-to-letter encoding, and string transmission
