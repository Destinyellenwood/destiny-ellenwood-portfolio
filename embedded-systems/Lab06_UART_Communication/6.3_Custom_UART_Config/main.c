// Lab 6.3 – UART with Custom ACLK Configuration
// Destiny Ellenwood – MSP430FR6989

#include <msp430fr6989.h>

#define redLED BIT0
#define greenLED BIT7

#define FLAGS UCA1IFG
#define RXFLAG UCRXIFG
#define TXFLAG UCTXIFG
#define TXBUFFER UCA1TXBUF
#define RXBUFFER UCA1RXBUF

void Initialize_UART_ACLK(void);
void config_ACLK_to_32KHz_crystal(void);
void uart_write_char(unsigned char ch);
void uart_write_string(char *str);
void uart_write_uint16(unsigned int n);
unsigned char uart_read_char(void);

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= redLED | greenLED;
    P1OUT |= redLED;
    P9OUT &= ~greenLED;

    Initialize_UART_ACLK();

    for (;;) {
        unsigned char input = uart_read_char();
        if (input == '1') {
            P9OUT |= greenLED;
        } else if (input == '2') {
            P9OUT &= ~greenLED;
        }

        uart_write_uint16(8457);
        uart_write_char('\n');
        uart_write_char('\r');
        uart_write_string("This class is challenging but fun!");
        uart_write_char('\n');
        uart_write_char('\r');

        P1OUT ^= redLED;
        __delay_cycles(200000);
    }
}

void config_ACLK_to_32KHz_crystal(void) {
    PJSEL1 &= ~BIT4;
    PJSEL0 |= BIT4;

    CSCTL0 = CSKEY;
    do {
        CSCTL5 &= ~LFXTOFFG;
        SFRIFG1 &= ~OFIFG;
    } while (CSCTL5 & LFXTOFFG);
    CSCTL0_H = 0;
}

void Initialize_UART_ACLK(void) {
    config_ACLK_to_32KHz_crystal();

    P3SEL1 &= ~(BIT4 | BIT5);
    P3SEL0 |= (BIT4 | BIT5);

    UCA1CTLW0 = UCSWRST;
    UCA1CTLW0 |= UCSSEL_1;   // ACLK

    UCA1BRW = 6;

    // Custom modulator values (no oversampling)
    UCA1MCTLW = UCBRS7 | UCBRS6 | UCBRS5 | UCBRS3 | UCBRS2 | UCBRS1;
    UCA1MCTLW &= ~UCOS16;

    UCA1CTLW0 &= ~UCSWRST;
}

void uart_write_uint16(unsigned int n) {
    if (n == 0) {
        uart_write_char('A');
        return;
    }

    unsigned int divisor = 10000;
    while (n < divisor) {
        divisor /= 10;
    }

    while (divisor > 0) {
        unsigned char digit = n / divisor;
        uart_write_char('A' + digit);
        n %= divisor;
        divisor /= 10;
    }
}

void uart_write_char(unsigned char ch) {
    while ((FLAGS & TXFLAG) == 0);
    TXBUFFER = ch;
}

void uart_write_string(char *str) {
    while (*str != '\0') {
        uart_write_char(*str++);
    }
}

unsigned char uart_read_char(void) {
    return (FLAGS & RXFLAG) ? RXBUFFER : 0;
}
