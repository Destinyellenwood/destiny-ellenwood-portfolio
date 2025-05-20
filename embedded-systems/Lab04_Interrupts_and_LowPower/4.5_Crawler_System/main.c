// Lab 4.5 – Crawler System with Finite-State Machine
// Destiny Ellenwood – MSP430FR6989

#include <msp430fr6989.h>

#define redLED    BIT0
#define greenLED  BIT7
#define BUT1      BIT1
#define BUT2      BIT2

// State definitions
#define RG                  0
#define R_PLUS              1
#define R_PLUS_PLUS         2
#define R_PLUS_PLUS_PLUS    3
#define G_PLUS              4
#define G_PLUS_PLUS         5
#define G_PLUS_PLUS_PLUS    6

volatile unsigned int state = RG;

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

    // LED setup
    P1DIR |= redLED;
    P9DIR |= greenLED;

    // Button setup
    P1DIR &= ~(BUT1 | BUT2);
    P1REN |= (BUT1 | BUT2);
    P1OUT |= (BUT1 | BUT2);
    P1IES |= (BUT1 | BUT2);
    P1IFG &= ~(BUT1 | BUT2);
    P1IE  |= (BUT1 | BUT2);

    config_ACLK_to_32KHz_crystal();

    // Timer setup
    TA0CCTL0 = CCIE;
    TA0CCR0 = 32768;
    TA0CTL = TASSEL_1 | MC_1 | TACLR;

    _enable_interrupts();

    while (1); // Main loop idle
}

// Timer ISR
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void) {
    switch (state) {
        case RG:
            P1OUT ^= redLED;
            P9OUT ^= greenLED;
            TA0CCR0 = 32768;
            break;
        case R_PLUS:
            P1OUT ^= redLED;
            P9OUT &= ~greenLED;
            TA0CCR0 = 16384;
            break;
        case R_PLUS_PLUS:
            P1OUT ^= redLED;
            P9OUT &= ~greenLED;
            TA0CCR0 = 8192;
            break;
        case R_PLUS_PLUS_PLUS:
            P1OUT ^= redLED;
            P9OUT &= ~greenLED;
            TA0CCR0 = 4096;
            break;
        case G_PLUS:
            P9OUT ^= greenLED;
            P1OUT &= ~redLED;
            TA0CCR0 = 16384;
            break;
        case G_PLUS_PLUS:
            P9OUT ^= greenLED;
            P1OUT &= ~redLED;
            TA0CCR0 = 8192;
            break;
        case G_PLUS_PLUS_PLUS:
            P9OUT ^= greenLED;
            P1OUT &= ~redLED;
            TA0CCR0 = 4096;
            break;
    }
}

// Button ISR
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void) {
    if (P1IFG & BUT1) {
        switch (state) {
            case RG: state = R_PLUS; break;
            case R_PLUS: state = R_PLUS_PLUS; break;
            case R_PLUS_PLUS: state = R_PLUS_PLUS_PLUS; break;
            case G_PLUS: state = RG; break;
            case G_PLUS_PLUS: state = G_PLUS; break;
            case G_PLUS_PLUS_PLUS: state = G_PLUS_PLUS; break;
        }
        __delay_cycles(20000);
        P1IFG &= ~BUT1;
    }

    if (P1IFG & BUT2) {
        switch (state) {
            case RG: state = G_PLUS; break;
            case R_PLUS: state = RG; break;
            case R_PLUS_PLUS: state = R_PLUS; break;
            case R_PLUS_PLUS_PLUS: state = R_PLUS_PLUS; break;
            case G_PLUS: state = G_PLUS_PLUS; break;
            case G_PLUS_PLUS: state = G_PLUS_PLUS_PLUS; break;
        }
        __delay_cycles(20000);
        P1IFG &= ~BUT2;
    }
}
