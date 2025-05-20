// Lab 8.2 – ADC12_B Sequenced Reading of Joystick X and Y
// Destiny Ellenwood – MSP430FR6989

#include <msp430fr6989.h>

#define FLAGS     UCA1IFG
#define TXFLAG    UCTXIFG
#define TXBUFFER  UCA1TXBUF
#define redLED    BIT0

void Initialize_UART(void);
void Initialize_ADC_XY(void);
void uart_write_char(unsigned char ch);
void uart_write_string(char *str);
void uart_write_uint16(unsigned int n);

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= redLED;
    P1OUT |= redLED;

    Initialize_UART();
    Initialize_ADC_XY();

    for (;;) {
        ADC12CTL0 |= ADC12ENC | ADC12SC;       // Start sampling/conversion
        while (ADC12CTL1 & ADC12BUSY);         // Wait until conversion done

        unsigned int joystick_x = ADC12MEM0;
        unsigned int joystick_y = ADC12MEM1;

        uart_write_string("X = ");
        uart_write_uint16(joystick_x);
        uart_write_string(" | Y = ");
        uart_write_uint16(joystick_y);
        uart_write_string("\r\n");

        P1OUT ^= redLED;
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

void Initialize_ADC_XY(void) {
    // X-axis on A10 (P9.2), Y-axis on A4 (P8.7)
    P9SEL1 |= BIT2;
    P9SEL0 |= BIT2;
    P8SEL1 |= BIT7;
    P8SEL0 |= BIT7;

    ADC12CTL0 |= ADC12ON;
    ADC12CTL0 &= ~ADC12ENC;

    ADC12CTL0 |= ADC12SHT0_2;
    ADC12CTL1 |= ADC12SHS_0 | ADC12SHP | ADC12DIV_0 | ADC12SSEL_1 | ADC12CONSEQ_1;
    ADC12CTL2 |= ADC12RES_2;
    ADC12CTL2 &= ~ADC12DF;

    ADC12MCTL0 = ADC12VRSEL_0 | ADC12INCH_10;
    ADC12MCTL1 = ADC12VRSEL_0 | ADC12INCH_4 | ADC12EOS;

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
