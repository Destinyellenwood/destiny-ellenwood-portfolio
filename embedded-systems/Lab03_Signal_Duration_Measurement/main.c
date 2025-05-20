// Lab 03 – Signal Duration Measurement using Timer A
// Destiny Ellenwood - MSP430FR6989

#include <msp430fr6989.h>

#define redLED    BIT0   // P1.0
#define greenLED  BIT7   // P9.7
#define S1        BIT1   // P1.1 - Start
#define S2        BIT2   // P1.2 - Reset

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

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= redLED;
    P9DIR |= greenLED;
    P1DIR &= ~(S1 | S2);      // Buttons as input
    P1REN |= (S1 | S2);       // Enable pull-up/down resistors
    P1OUT |= (S1 | S2);       // Pull-up resistors

    config_ACLK_to_32KHz_crystal();

    while (1) {
        // Wait for S1 press
        while (P1IN & S1);

        // Start Timer in continuous mode
        TA0CTL = TASSEL_1 | MC_2 | TACLR;

        // Wait for S1 release
        while (!(P1IN & S1));

        // Stop timer
        TA0CTL &= ~MC_3;

        if (TA0CTL & TAIFG) {
            // Overflow occurred → duration too long
            P9OUT |= greenLED;

            // Wait for reset button S2 press
            while (P1IN & S2);
            P9OUT &= ~greenLED;
            TA0CTL &= ~TAIFG;
        } else {
            // Save the time into CCR0
            TA0CCR0 = TA0R;

            // Start timer in up mode
            TA0CTL = TASSEL_1 | MC_1 | TACLR;
            P1OUT |= redLED;

            // Wait for timer to finish
            while (!(TA0CTL & TAIFG));

            // End pulse
            P1OUT &= ~redLED;
            TA0CTL &= ~MC_1;
            TA0CTL &= ~TAIFG;
        }
    }
}
