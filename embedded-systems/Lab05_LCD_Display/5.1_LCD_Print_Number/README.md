# Lab 5.1 – Printing Numbers to LCD (MSP430FR6989)

## Overview
This part of the lab introduces the MSP430’s LCD_C module by printing a hardcoded 16-bit number (`12345`) onto the 5-digit LCD display. It uses segment-mapped memory registers and a digit decoding array.

## Objectives
- Configure LCD_C using the 32.768 KHz LFXT clock
- Map digits to LCD segment memory (LCDMx)
- Use delay loop and LED blinking to verify run state

## Key Features
- 5-digit LCD mapping: A6 → A2
- Red LED blinks using a delay loop
- LCD segments are updated using bit patterns per digit

## Source Files
- `main.c` – Contains initialization, number-to-LCD conversion, and blinking loop
