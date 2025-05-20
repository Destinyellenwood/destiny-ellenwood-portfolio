// Lab 01 - Flashing Two LEDs
// Destiny Ellenwood - UCF
// Microcontroller: MSP430FR6989

#include <msp430fr6989.h>
#include <stdint.h>

#define redLED    BIT0   // P1.0
#define greenLED  BIT7   // P9.7

void main(void) {
    volatile uint32_t i; // 32-bit delay counter

    // Stop Watchdog Timer
    WDTCTL = WDTPW | WDTHOLD;

    // Unlock GPIO
    PM5CTL0 &= ~LOCKLPM5;

    // Configure LED pins as outputs
    P1DIR |= redLED;
    P9DIR |= greenLED;

    // Turn off LEDs initially
    P1OUT &= ~redLED;
    P9OUT &= ~greenLED;

    while (1) {
        // Toggle both LEDs
        P1OUT ^= redLED;
        P9OUT ^= greenLED;

        // Delay loop (~long delay)
        for (i = 0; i < 80000; i++) {}

        // Optional: Add _delay_cycles for consistency
        _delay_cycles(3000000);
    }
}
