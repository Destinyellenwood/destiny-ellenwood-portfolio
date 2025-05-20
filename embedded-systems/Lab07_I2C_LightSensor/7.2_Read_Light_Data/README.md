# Lab 7.2 – Light Sensor Data Acquisition via I2C (MSP430FR6989)

## Overview
This program configures the OPT3001 light sensor via I2C and reads real-time ambient light data in lux. The lux values are converted and sent over UART, allowing you to observe live sensor output on a terminal.

---

## Objectives
- Configure the OPT3001 using register 0x01
- Read light measurements from register 0x00
- Convert raw data into lux using datasheet scaling
- Output results through UART

---

## Behavior
- Sensor configured to continuous conversion mode (0x7400)
- Every second, the MSP430:
  - Reads the lux result register (0x00)
  - Converts to lux using `lux = raw * 1.28`
  - Sends the result over UART

Example Output:

OPT3001 Light Sensor Test
Reading #1: Lux = 121
Reading #2: Lux = 135

---

## Key Concepts
- 16-bit I2C read/write with register addressing
- I2C repeated start and STOP management
- UART formatted output of floating-point approximations
- Real-time sensor data streaming

---

## Source Files
- `main.c` — Contains UART, I2C, sensor config, and lux reading loop
