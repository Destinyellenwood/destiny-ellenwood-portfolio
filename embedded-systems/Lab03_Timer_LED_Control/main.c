// Lab 03 – Flashing LED with Timer A in Continuous Mode
// Destiny Ellenwood - MSP430FR6989

#include <msp430fr6989.h>

#define redLED BIT0
#define greenLED BIT7

// Configure ACLK to use 32.768 KHz crystal
void config_ACLK_to_32KHz_crystal() {
    PJSEL1 &= ~BIT4;
    PJSEL0 |= BIT4;

    CSCTL0 = CSKEY;  // Unlock clock system
    do {
        CSCTL5 &= ~LFXTOFFG;   // Clear local oscillator fault
        SFRIFG1 &= ~OFIFG;     // Clear global fault
    } while (CSCTL5 & LFXTOFFG);
    CSCTL0_H = 0; // Lock CS registers
}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;  // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;      // Unlock GPIO

    P1DIR |= redLED;
    P9DIR |= greenLED;
    P1OUT &= ~redLED;
    P9OUT &= ~greenLED;

    config_ACLK_to_32KHz_crystal();  // Set ACLK

    // Configure Timer A for continuous mode using ACLK
    TA0CTL = TASSEL_1 | ID_0 | MC_2 | TACLR;  // ACLK | /1 | Continuous mode | Clear TAR

    TA0CTL &= ~TAIFG; // Clear interrupt flag

    while (1) {
        // Wait for overflow
        while (!(TA0CTL & TAIFG)) {}

        P1OUT ^= redLED;      // Toggle red LED
        TA0CTL &= ~TAIFG;     // Clear flag
    }
}
