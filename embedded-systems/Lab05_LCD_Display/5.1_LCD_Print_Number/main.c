// Lab 5.1 – LCD Display: Print a 16-bit number
// Destiny Ellenwood – MSP430FR6989

#include <msp430fr6989.h>

#define redLED BIT0
#define greenLED BIT7

void Initialize_LCD();
void lcd_write_uint16(unsigned int n);

const unsigned char LCD_Shapes[10] = {
    0xFC, 0x60, 0xDB, 0xF3, 0x67,
    0xB7, 0xBF, 0xE4, 0xFF, 0xF7
};

void main(void) {
    volatile unsigned int i;
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= redLED;
    P9DIR |= greenLED;
    P1OUT |= redLED;
    P9OUT &= ~greenLED;

    Initialize_LCD();
    lcd_write_uint16(12345);

    while (1) {
        for (i = 0; i <= 60000; i++);
        P1OUT ^= redLED;
    }
}

void lcd_write_uint16(unsigned int n) {
    unsigned char digits[5] = {0};
    int i;

    for (i = 0; i < 5 && n > 0; i++) {
        digits[i] = n % 10;
        n /= 10;
    }

    volatile unsigned char* const LCDM_map[5] = {
        &LCDM8, &LCDM15, &LCDM19, &LCDM4, &LCDM6
    };

    for (int j = 0; j < 5; j++) {
        if (j < i)
            *LCDM_map[j] = LCD_Shapes[digits[j]];
        else
            *LCDM_map[j] = 0;
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
