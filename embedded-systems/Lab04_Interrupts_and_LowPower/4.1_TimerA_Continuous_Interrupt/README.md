# Lab 4.1 – Timer A Continuous Mode with Interrupt (MSP430FR6989)

## Overview
This experiment demonstrates how to use Timer A in continuous mode with an interrupt service routine (ISR) to blink an LED. The timer overflows every 2 seconds (based on a 32.768 KHz crystal), triggering an interrupt.

## Objectives
- Configure Timer A for continuous mode
- Enable the TAIE interrupt
- Use `TAIFG` flag to trigger periodic toggling
- Execute a low-power idle loop between interrupts

## Key Concepts
- Hardware-triggered ISRs
- Timer A overflow interrupt vector: `TIMER0_A1_VECTOR`
- Power-aware interrupt toggling

## Behavior
- Red LED on P1.0 toggles every ~2 seconds based on ACLK
- Code sits in a `while(1)` loop while ISR handles blinking

## Source Files
- `main.c` — Sets up Timer A, ISR, and ACLK configuration
