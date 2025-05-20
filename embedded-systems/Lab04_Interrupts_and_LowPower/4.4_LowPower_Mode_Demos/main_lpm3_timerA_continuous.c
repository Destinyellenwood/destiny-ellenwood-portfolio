// Lab 4.4 – LPM3 with Timer A Continuous Mode Interrupt
// Destiny Ellenwood – MSP430FR6989

#include <msp430fr6989.h>

#define redLED    BIT0
#define greenLED  BIT7

#pragma vector = TIMER0_A1_VECTOR
__interrupt void T0A1_ISR(void) {
    P1OUT ^= redLED;
    TA0CTL &= ~TAIFG;
}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= redLED | greenLED;
    P1OUT &= ~(redLED | greenLED);

    // Configure Timer A
    TA0CTL = TASSEL_1 | ID_0 | MC_2 | TACLR | TAIE; // ACLK | continuous mode
    TA0CTL &= ~TAIFG;

    __enable_interrupt();

    _low_power_mode_3(); // Enter LPM3 — ACLK active, CPU halted
}

