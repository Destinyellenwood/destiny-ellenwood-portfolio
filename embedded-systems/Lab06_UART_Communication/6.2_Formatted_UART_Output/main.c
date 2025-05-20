// Lab 6.2 – Formatted UART Output (Strings and Integers)
// Destiny Ellenwood – MSP430FR6989

#include <msp430fr6989.h>

#define redLED BIT0
#define greenLED BIT7

#define FLAGS UCA1IFG
#define RXFLAG UCRXIFG
#define TXFLAG UCTXIFG
#define TXBUFFER UCA1TXBUF
#define RXBUFFER UCA1RXBUF

void Initialize_UART(void);
void uart_write_char(unsigned char ch);
void uart_write_uint16(unsigned int n);
void uart_write_string(char *str);
unsigned char uart_read_char(void);

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= redLED | greenLED;
    P1OUT |= redLED;
    P9OUT &= ~greenLED;

    Initialize_UART();

    for (;;) {
        unsigned char input = uart_read_char();

        if (input == '1') {
            P9OUT |= greenLED;
        } else if (input == '2') {
            P9OUT &= ~greenLED;
        }

        P1OUT ^= redLED;

        uart_write_uint16(10287);      // Sends A-C-B-H
        uart_write_char('\n');
        uart_write_char('\r');
        uart_write_string("Hello UART!");
        uart_write_char('\n');
        uart_write_char('\r');

        __delay_cycles(200000); // Slow output a bit
    }
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

void uart_write_string(char *str) {
    while (*str != '\0') {
        uart_write_char(*str++);
    }
}

void uart_write_char(unsigned char ch) {
    while ((FLAGS & TXFLAG) == 0);
    TXBUFFER = ch;
}

unsigned char uart_read_char(void) {
    return (FLAGS & RXFLAG) ? RXBUFFER : 0;
}

void Initialize_UART(void) {
    P3SEL1 &= ~(BIT4 | BIT5);
    P3SEL0 |= (BIT4 | BIT5);

    UCA1CTLW0 = UCSWRST;
    UCA1CTLW0 |= UCSSEL_2; // SMCLK
    UCA1BRW = 6;
    UCA1MCTLW = UCBRF_8 | UCBRS5 | UCOS16;
    UCA1CTLW0 &= ~UCSWRST;
}
