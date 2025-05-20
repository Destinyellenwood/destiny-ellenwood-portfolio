// Lab 6.1 – Basic UART Communication
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
unsigned char uart_read_char(void);

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= redLED | greenLED;
    P1OUT |= redLED;
    P9OUT &= ~greenLED;

    Initialize_UART();

    unsigned char i = '0';

    for (;;) {
        for (i = '0'; i <= '9'; i++) {
            uart_write_char(i);
            uart_write_char(10);  // LF
            uart_write_char(13);  // CR

            P1OUT ^= redLED;

            unsigned char input = uart_read_char();
            if (input == '1') {
                P9OUT |= greenLED;
            } else if (input == '2') {
                P9OUT &= ~greenLED;
            }

            __delay_cycles(100000);
        }
    }
}

void Initialize_UART(void) {
    P3SEL1 &= ~(BIT4 | BIT5);
    P3SEL0 |= (BIT4 | BIT5);

    UCA1CTLW0 = UCSWRST;
    UCA1CTLW0 |= UCSSEL_2;      // SMCLK
    UCA1BRW = 6;
    UCA1MCTLW = UCBRF_8 | UCBRS5 | UCOS16;
    UCA1CTLW0 &= ~UCSWRST;
}

void uart_write_char(unsigned char ch) {
    while ((FLAGS & TXFLAG) == 0);
    TXBUFFER = ch;
}

unsigned char uart_read_char(void) {
    return (FLAGS & RXFLAG) ? RXBUFFER : 0;
}
