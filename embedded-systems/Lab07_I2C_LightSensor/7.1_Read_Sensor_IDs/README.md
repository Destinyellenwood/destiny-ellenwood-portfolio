# Lab 7.1 – Reading Manufacturer and Device IDs via I2C (MSP430FR6989)

## Overview
This program demonstrates how to use the MSP430FR6989 to communicate over I2C with the OPT3001 ambient light sensor and retrieve two key hardware identifiers:

- **Manufacturer ID** from register `0x7E`
- **Device ID** from register `0x7F`

The data is then sent over UART to a connected terminal for debugging and confirmation.

---

## Objectives
- Initialize the eUSCI_B1 module in I2C master mode
- Communicate with I2C device at address `0x44`
- Read 16-bit data from sensor registers
- Format and transmit the result over UART using eUSCI_A1

---

## Behavior
After initialization, the MSP430 sends the following output to the UART terminal every second:

OPT3001 Light Sensor Test
Manufacturer ID: 0x5449
Device ID: 0x3001


The values `0x5449` and `0x3001` are typical for the Texas Instruments OPT3001 sensor.

---

## Hardware Connections
- **I2C SDA**: P4.1  
- **I2C SCL**: P4.0  
- **UART TX**: P3.4  
- **UART RX**: P3.5  
- **Red LED**: P1.0 (toggles after each read)

---

## Key Concepts
- Multi-byte read operations using I2C start → write → repeated start → read
- Using `UCB1CTLW0`, `UCB1I2CSA`, and buffer flags (`UCTXIFG0`, `UCRXIFG0`)
- Combining two 8-bit reads into a 16-bit register value
- Hexadecimal formatting via bit shifts and masking

---

## Source Files
- `main.c` — I2C and UART initialization, device register reading, formatted output
