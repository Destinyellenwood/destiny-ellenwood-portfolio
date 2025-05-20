# Lab 8.1 – Reading X-Axis from Joystick using ADC12_B (MSP430FR6989)

## Overview
This program demonstrates how to read the X-axis value from a joystick using the SAR-type ADC (ADC12_B) on the MSP430FR6989. The result is sent over UART to a terminal at 9600 baud.

---

## Objectives
- Configure ADC to read analog input A10 (X-axis)
- Set sample-and-hold time using ADC12SHT0
- Read 12-bit result using polling
- Transmit ADC result to PC using UART

---

## Behavior
- Reads ADC from A10 (P9.2)
- Converts analog voltage to digital 12-bit value (0–4095)
- Sends result over UART once per second

---

## Hardware
- Joystick X-axis input: A10 / P9.2
- UART TX: P3.4, RX: P3.5
- Red LED: P1.0 (basic status indicator)

---

## Source Files
- `main.c` — ADC and UART setup, X-axis ADC reading loop
