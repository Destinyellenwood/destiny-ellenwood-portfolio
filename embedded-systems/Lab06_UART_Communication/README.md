# Lab 06 – UART Communication (MSP430FR6989)

## Overview
This lab introduces Universal Asynchronous Receiver/Transmitter (UART) communication using the MSP430FR6989. It covers both basic character transmission and more advanced formatted output using C functions. The UART peripheral (eUSCI_A1) is configured for various clock sources and baud rate settings.

Students implemented both transmit and receive operations, developed functions to output integers and strings, and experimented with different UART clock configurations (SMCLK and ACLK).

---

## Key Concepts
- eUSCI_A1 UART module configuration
- 9600 baud UART with 8-N-1 format
- Transmitting characters, strings, and integers
- UART read/write buffers and flags
- Clock source control (SMCLK vs ACLK)
- Backchannel UART via P3.4 (TX) and P3.5 (RX)

---

## Subprojects

| Subfolder | Description |
|-----------|-------------|
| [6.1_Basic_Transmit_Receive](./6.1_Basic_Transmit_Receive/) | Transmits digits 0–9; reads user input to control LEDs |
| [6.2_Formatted_UART_Output](./6.2_Formatted_UART_Output/) | Transmits integers as letter codes and strings like "Hello UART!" |
| [6.3_Custom_UART_Config](./6.3_Custom_UART_Config/) | Uses ACLK as UART clock source; sends messages and responds to user input |

---

## Hardware Used
- **Red LED**: P1.0 (toggle indicator)
- **Green LED**: P9.7 (status control via UART input)
- **UART TX**: P3.4
- **UART RX**: P3.5

---

## Skills Demonstrated
- Peripheral initialization and UART configuration
- Handling UART transmit/receive flags (`UCTXIFG`, `UCRXIFG`)
- Writing modular UART I/O functions in C
- Serial debugging and terminal communication (using backchannel UART)

This lab simulates how real embedded systems communicate with host devices, sensors, or external interfaces in a clean, interrupt-free polling environment.
