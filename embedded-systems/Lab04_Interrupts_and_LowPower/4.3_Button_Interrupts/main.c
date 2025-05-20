// Lab 4.3 – Push Button Interrupts
// Destiny Ellenwood – MSP430FR6989

#include <msp430fr6989.h>

#define redLED    BIT0   // P1.0
#define greenLED  BIT7   // P9.7
#define BUT1      BIT1   // P1.1
#define BUT2      BIT2   // P1.2

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;    // Stop Watchdog timer
    PM5CTL0 &= ~LOCKLPM5;        // Enable GPIO

    // Set LED pins as output
    P1DIR |= redLED;
    P9DIR |= greenLED;
    P1OUT &= ~redLED;
    P9OUT &= ~greenLED;

    // Set buttons as input with pull-up resistors
    P1DIR &= ~(BUT1 | BUT2);
    P1REN |= (BUT1 | BUT2);
    P1OUT |= (BUT1 | BUT2);       // Pull-up

    // Enable interrupts on falling edge
    P1IES |= (BUT1 | BUT2);
    P1IFG &= ~(BUT1 | BUT2);      // Clear flags
    P1IE  |= (BUT1 | BUT2);       // Enable interrupts

    __enable_interrupt();         // Enable global interrupts

    while (1); // Idle loop
}

// ISR for Port 1
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void) {
    if (P1IFG & BUT1) {
        P1OUT ^= redLED;
        P1IFG &= ~BUT1; // Clear flag
    }

    if (P1IFG & BUT2) {
        P9OUT ^= greenLED;
        P1IFG &= ~BUT2; // Clear flag
    }
}
