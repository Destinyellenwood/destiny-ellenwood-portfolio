// Lab 5.2 – LCD Counter with Button Control
// Destiny Ellenwood – MSP430FR6989

#include <msp430fr6989.h>

#define redLED BIT0
#define greenLED BIT7
#define S1 BIT1
#define S2 BIT2

volatile unsigned int counter = 0;

void Initialize_LCD();
void lcd_write_uint16(unsigned int n);

const unsigned char LCD_Shapes[10] = {
    0xFC, 0x60, 0xDB, 0xF3, 0x67,
    0xB7, 0xBF, 0xE4, 0xFF, 0xF7
};

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    // LED setup
    P1DIR |= redLED | greenLED;
    P1OUT &= ~(redLED | greenLED);

    // Button setup
    P1DIR &= ~(S1 | S2);
    P1REN |= (S1 | S2);
    P1OUT |= (S1 | S2);
    P1IE |= (S1 | S2);
    P1IES |= (S1 | S2);
    P1IFG &= ~(S1 | S2);

    // Timer setup
    TA0CCTL0 = CCIE;
    TA0CCR0 = 32768 - 1;
    TA0CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;

    Initialize_LCD();
    __enable_interrupt();

    while (1) {
        lcd_write_uint16(counter);
        __low_power_mode_3(); // Sleep between events
    }
}

// Timer ISR — 1 second interval
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A0_ISR(void) {
    counter++;
    if (counter > 65535)
        counter = 0;

    TA0CCTL0 &= ~CCIFG;
    __low_power_mode_off_on_exit();
}

// Button ISR — Reset or increment
#pragma vector = PORT1_VECTOR
__interrupt void Port_1_ISR(void) {
    if (P1IFG & S1) {
        counter = 0;
    }
    if (P1IFG & S2) {
        counter += 1000;
        if (counter > 65535)
            counter %= 65536;
    }

    P1IFG &= ~(S1 | S2);
    __low_power_mode_off_on_exit();
}

void lcd_write_uint16(unsigned int n) {
    unsigned char digits[5] = {0};
    int i, j;

    for (i = 0; i < 5 && n > 0; i++) {
        digits[i] = n % 10;
        n /= 10;
    }

    volatile unsigned char* const LCDM_map[5] = {
        &LCDM8, &LCDM15, &LCDM19, &LCDM4, &LCDM6
    };

    for (j = 0; j < 5; j++) {
        *LCDM_map[j] = (j < i) ? LCD_Shapes[digits[j]] : 0;
    }
}

void Initialize_LCD() {
    PJSEL0 = BIT4 | BIT5;

    LCDCPCTL0 = 0xFFD0;
    LCDCPCTL1 = 0xF83F;
    LCDCPCTL2 = 0x00F8;

    CSCTL0_H = CSKEY >> 8;
    CSCTL4 &= ~LFXTOFF;
    do {
        CSCTL5 &= ~LFXTOFFG;
        SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);
    CSCTL0_H = 0;

    LCDCCTL0 = LCDDIV__1 | LCDPRE__16 | LCD4MUX | LCDLP;
    LCDCVCTL = VLCD_1 | VLCDREF_0 | LCDCPEN;
    LCDCCPCTL = LCDCPCLKSYNC;
    LCDCMEMCTL = LCDCLRM;
    LCDCCTL0 |= LCDON;
}
