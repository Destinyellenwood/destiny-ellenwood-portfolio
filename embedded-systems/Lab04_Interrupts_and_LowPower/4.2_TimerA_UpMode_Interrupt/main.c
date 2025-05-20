// Lab 4.2 – Timer A Up Mode with Interrupt
// Destiny Ellenwood – MSP430FR6989

#include <msp430fr6989.h>

#define redLED    BIT0
#define greenLED  BIT7

void config_ACLK_to_32KHz_crystal() {
    PJSEL1 &= ~BIT4;
    PJSEL0 |= BIT4;

    CSCTL0 = CSKEY;
    do {
        CSCTL5 &= ~LFXTOFFG;
        SFRIFG1 &= ~OFIFG;
    } while (CSCTL5 & LFXTOFFG);
    CSCTL0_H = 0;
}

// Timer A interrupt for CCR0
#pragma vector = TIMER0_A0_VECTOR
__interrupt void T0A0_ISR(void) {
    P1OUT ^= redLED;     // Toggle red LED
    P9OUT ^= greenLED;   // Toggle green LED
}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= redLED;
    P9DIR |= greenLED;
    P1OUT &= ~redLED;
    P9OUT |= greenLED;   // Green LED initially on

    config_ACLK_to_32KHz_crystal();

    TA0CCR0 = 32768;          // ~1 second at 32.768 KHz
    TA0CCTL0 = CCIE;          // Enable interrupt for CCR0
    TA0CCTL0 &= ~CCIFG;       // Clear interrupt flag

    TA0CTL = TASSEL_1 | MC_1 | TACLR; // ACLK, Up mode, Clear TAR

    __enable_interrupt();     // Global interrupt enable

    while (1); // Idle loop
}
