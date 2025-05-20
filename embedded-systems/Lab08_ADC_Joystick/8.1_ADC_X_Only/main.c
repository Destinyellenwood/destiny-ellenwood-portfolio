// Lab 8.1 – Reading X-axis of Joystick with ADC12_B
// Destiny Ellenwood – MSP430FR6989

#include <msp430fr6989.h>

#define FLAGS     UCA1IFG
#define TXFLAG    UCTXIFG
#define TXBUFFER  UCA1TXBUF
#define redLED    BIT0

void Initialize_UART(void);
void Initialize_ADC_X(void);
void uart_write_char(unsigned char ch);
void uart_write_string(char *str);
void uart_write_uint16(unsigned int n);

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= redLED;
    P1OUT |= redLED;

    Initialize_UART();
    Initialize_ADC_X();

    for (;;) {
        ADC12CTL0 |= ADC12ENC | ADC12SC;         // Start conversion
        while (ADC12CTL1 & ADC12BUSY);           // Wait for it to finish
        unsigned int joystick_x = ADC12MEM0;     // Read result

        uart_write_string("Joystick X-Axis ADC Value: ");
        uart_write_uint16(joystick_x);
        uart_write_string("\r\n");

        __delay_cycles(1000000);
    }
}

void Initialize_UART(void) {
    P3SEL1 &= ~(BIT4 | BIT5);
    P3SEL0 |=  (BIT4 | BIT5);

    UCA1CTLW0 = UCSWRST;
    UCA1CTLW0 |= UCSSEL_2;
    UCA1BRW = 6;
    UCA1MCTLW = UCBRF_8 | UCBRS5 | UCOS16;
    UCA1CTLW0 &= ~UCSWRST;
}

void Initialize_ADC_X(void) {
    P9SEL1 |= BIT2;
    P9SEL0 |= BIT2;

    ADC12CTL0 |= ADC12ON;
    ADC12CTL0 &= ~ADC12ENC;

    ADC12CTL0 |= ADC12SHT0_2;
    ADC12CTL1 |= ADC12SHS_0 | ADC12SHP | ADC12DIV_0 | ADC12SSEL_1;
    ADC12CTL2 |= ADC12RES_2;
    ADC12CTL2 &= ~ADC12DF;
    ADC12MCTL0 = ADC12VRSEL_0 | ADC12INCH_10;

    ADC12CTL0 |= ADC12ENC;
}

void uart_write_char(unsigned char ch) {
    while (!(FLAGS & TXFLAG));
    TXBUFFER = ch;
}

void uart_write_string(char *str) {
    while (*str) {
        uart_write_char(*str++);
    }
}

void uart_write_uint16(unsigned int n) {
    char buffer[6];
    int i = 5;
    buffer[i--] = '\0';

    do {
        buffer[i--] = '0' + (n % 10);
        n /= 10;
    } while (n > 0);

    uart_write_string(&buffer[i + 1]);
}
