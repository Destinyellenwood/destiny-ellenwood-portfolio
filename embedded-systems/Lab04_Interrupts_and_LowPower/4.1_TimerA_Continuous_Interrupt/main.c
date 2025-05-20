// Lab 4.1 – Timer A Continuous Mode with Interrupt
// Destiny Ellenwood - MSP430FR6989

#include <msp430fr6989.h>

#define redLED BIT0
#define greenLED BIT7

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

// Timer A interrupt vector
#pragma vector = TIMER0_A1_VECTOR
__interrupt void T0A1_ISR() {
    P1OUT ^= redLED;
    TA0CTL &= ~TAIFG; // Clear interrupt flag
}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= redLED;
    P9DIR |= greenLED;
    P1OUT &= ~redLED;
    P9OUT &= ~greenLED;

    config_ACLK_to_32KHz_crystal();

    // Timer A: ACLK | div 1 | continuous mode | enable interrupt
    TA0CTL = TASSEL_1 | ID_0 | MC_2 | TACLR | TAIE;
    TA0CTL &= ~TAIFG;

    __enable_interrupt();  // Enable global interrupts

    while (1);  // Idle loop
}
