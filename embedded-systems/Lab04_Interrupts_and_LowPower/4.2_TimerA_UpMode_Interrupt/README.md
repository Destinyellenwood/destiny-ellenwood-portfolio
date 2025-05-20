# Lab 4.2 – Timer A Up Mode Interrupt (MSP430FR6989)

## Overview
This experiment demonstrates the use of Timer A in up mode with a compare match interrupt to toggle two LEDs. It uses the 32.768 KHz ACLK crystal for accurate 1-second timing.

## Objectives
- Configure Timer A to run in up mode
- Enable interrupts on compare match (CCR0)
- Use `TIMER0_A0_VECTOR` ISR to toggle LEDs

## Key Concepts
- Timer up mode: counts from 0 to TA0CCR0
- `TA0CCTL0` is used to enable compare match interrupts
- ISR executes when TAR reaches CCR0

## Behavior
- Red LED (P1.0) and green LED (P9.7) toggle every ~1 second
- System sits in idle mode between interrupts

## Source Files
- `main.c` — Timer A setup in up mode with CCR0 interrupt
