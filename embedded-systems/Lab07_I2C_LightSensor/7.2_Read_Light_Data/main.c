// Lab 7.2 – Read and Convert Lux Values from OPT3001 via I2C
// Destiny Ellenwood – MSP430FR6989

#include <msp430fr6989.h>

#define FLAGS     UCA1IFG
#define RXFLAG    UCRXIFG
#define TXFLAG    UCTXIFG
#define TXBUFFER  UCA1TXBUF
#define RXBUFFER  UCA1RXBUF

#define redLED    BIT0  // P1.0

// Function Prototypes
void Initialize_UART(void);
void Initialize_I2C(void);
void uart_write_char(unsigned char ch);
void uart_write_string(char *str);
void uart_write_uint16(unsigned int n);
int i2c_read_word(unsigned char address, unsigned char reg, unsigned int *data);
int i2c_write_word(unsigned char address, unsigned char reg, unsigned int data);

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= redLED;
    P1OUT |= redLED;

    Initialize_UART();
    Initialize_I2C();

    unsigned int data = 0;
    unsigned int counter = 0;

    uart_write_string("OPT3001 Light Sensor Test\r\n");
    uart_write_string("------------------------\r\n");

    // Configure sensor (0x7400: RN=7, CT=0, M=3, ME=1)
    i2c_write_word(0x44, 0x01, 0x7400);

    for (;;) {
        counter++;

        // Read Result Register (0x00)
        i2c_read_word(0x44, 0x00, &data);

        unsigned int lux_raw = data & 0x0FFF;
        float lux = lux_raw * 1.28;  // LSB = 1.28 lux per count

        // UART Output
        uart_write_string("Reading #");
        uart_write_uint16(counter);
        uart_write_string(": Lux = ");

        int int_lux = (int)lux;
        uart_write_uint16(int_lux);
        uart_write_string(" lux\r\n");

        P1OUT ^= redLED;
        __delay_cycles(1000000);  // 1 second delay
    }
}

// UART Setup
void Initialize_UART(void) {
    P3SEL1 &= ~(BIT4 | BIT5);
    P3SEL0 |=  (BIT4 | BIT5);

    UCA1CTLW0 = UCSWRST;
    UCA1CTLW0 |= UCSSEL_2;  // SMCLK
    UCA1BRW = 6;
    UCA1MCTLW = UCBRF_8 | UCBRS5 | UCOS16;
    UCA1CTLW0 &= ~UCSWRST;
}

// I2C Setup
void Initialize_I2C(void) {
    P4SEL1 |=  (BIT1 | BIT0);
    P4SEL0 &= ~(BIT1 | BIT0);

    UCB1CTLW0 = UCSWRST;
    UCB1CTLW0 |= UCMST | UCMODE_3 | UCSYNC | UCSSEL_3;
    UCB1BRW = 8;
    UCB1CTLW0 &= ~UCSWRST;
}

// Write single char via UART
void uart_write_char(unsigned char ch) {
    while (!(FLAGS & TXFLAG));
    TXBUFFER = ch;
}

// Write string via UART
void uart_write_string(char *str) {
    while (*str) {
        uart_write_char(*str++);
    }
}

// Write unsigned integer via UART
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

// I2C read 16-bit value from a register
int i2c_read_word(unsigned char addr, unsigned char reg, unsigned int *data) {
    unsigned char msb = 0, lsb = 0;

    UCB1I2CSA = addr;
    UCB1IFG &= ~UCTXIFG0;

    // Write register
    UCB1CTLW0 |= UCTR | UCTXSTT;
    while (!(UCB1IFG & UCTXIFG0));
    UCB1TXBUF = reg;
    while (!(UCB1IFG & UCTXIFG0));

    // Read data
    UCB1CTLW0 &= ~UCTR;
    UCB1CTLW0 |= UCTXSTT;
    while (!(UCB1IFG & UCRXIFG0));
    msb = UCB1RXBUF;

    UCB1CTLW0 |= UCTXSTP;
    while (!(UCB1IFG & UCRXIFG0));
    lsb = UCB1RXBUF;

    while (UCB1CTLW0 & UCTXSTP);
    while (UCB1STATW & UCBBUSY);

    *data = ((unsigned int)msb << 8) | lsb;
    return 0;
}

// I2C write 16-bit value to a register
int i2c_write_word(unsigned char addr, unsigned char reg, unsigned int data) {
    unsigned char msb = (data >> 8) & 0xFF;
    unsigned char lsb = data & 0xFF;

    UCB1I2CSA = addr;
    UCB1IFG &= ~UCTXIFG0;

    UCB1CTLW0 |= UCTR | UCTXSTT;
    while (!(UCB1IFG & UCTXIFG0));
    UCB1TXBUF = reg;
    while (!(UCB1IFG & UCTXIFG0));
    UCB1TXBUF = msb;
    while (!(UCB1IFG & UCTXIFG0));
    UCB1TXBUF = lsb;
    while (!(UCB1IFG & UCTXIFG0));

    UCB1CTLW0 |= UCTXSTP;
    while (UCB1CTLW0 & UCTXSTP);
    while (UCB1STATW & UCBBUSY);

    return 0;
}
