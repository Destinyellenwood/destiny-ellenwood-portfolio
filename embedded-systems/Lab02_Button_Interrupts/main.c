// Lab 02 – Two Push Buttons to Control Two LEDs
// Destiny Ellenwood - MSP430FR6989

#include <msp430fr6989.h>

#define redLED    BIT0    // P1.0
#define greenLED  BIT7    // P9.7
#define BUT1      BIT1    // Button S1 at P1.1
#define BUT2      BIT2    // Button S2 at P1.2

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;  // Stop Watchdog timer
    PM5CTL0 &= ~LOCKLPM5;      // Enable GPIOs

    // LED setup
    P1DIR |= redLED;
    P9DIR |= greenLED;
    P1OUT &= ~redLED;
    P9OUT &= ~greenLED;

    // Button setup
    P1DIR &= ~(BUT1 | BUT2);     // Set P1.1 and P1.2 as input
    P1REN |= (BUT1 | BUT2);      // Enable internal resistors
    P1OUT |= (BUT1 | BUT2);      // Pull-up configuration

    while (1) {
        // Button S1 pressed → red LED ON
        if ((P1IN & BUT1) == 0)
            P1OUT |= redLED;
        else
            P1OUT &= ~redLED;

        // Button S2 pressed → green LED ON
        if ((P1IN & BUT2) == 0)
            P9OUT |= greenLED;
        else
            P9OUT &= ~greenLED;
    }
}
