// Lab 4.4 – LPM3 with Button Interrupts
// Destiny Ellenwood – MSP430FR6989

#include <msp430fr6989.h>

#define redLED    BIT0
#define greenLED  BIT7
#define BUT1      BIT1
#define BUT2      BIT2

#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void) {
    if (P1IFG & BUT1) {
        P1OUT ^= redLED;
        P1IFG &= ~BUT1;
    }
    if (P1IFG & BUT2) {
        P9OUT ^= greenLED;
        P1IFG &= ~BUT2;
    }
}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= redLED;
    P9DIR |= greenLED;
    P1OUT &= ~redLED;
    P9OUT &= ~greenLED;

    // Configure buttons
    P1DIR &= ~(BUT1 | BUT2);
    P1REN |= (BUT1 | BUT2);
    P1OUT |= (BUT1 | BUT2);
    P1IES |= (BUT1 | BUT2);
    P1IFG &= ~(BUT1 | BUT2);
    P1IE |= (BUT1 | BUT2);

    __enable_interrupt();

    _low_power_mode_3();
}
