// Lab 4.4 – LPM3 with Timer A Up Mode Interrupt
// Destiny Ellenwood – MSP430FR6989

#include <msp430fr6989.h>

#define redLED    BIT0
#define greenLED  BIT7

#pragma vector = TIMER0_A0_VECTOR
__interrupt void T0A0_ISR(void) {
    P1OUT ^= redLED;
    P9OUT ^= greenLED;
}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= redLED;
    P9DIR |= greenLED;
    P1OUT &= ~redLED;
    P9OUT |= greenLED;

    TA0CCR0 = 32768;
    TA0CCTL0 |= CCIE;
    TA0CCTL0 &= ~CCIFG;

    TA0CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;

    __enable_interrupt();

    _low_power_mode_3();
}
