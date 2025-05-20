// Lab 7.1 – Read Manufacturer and Device IDs via I2C
// Destiny Ellenwood – MSP430FR6989

#include <msp430fr6989.h>

// UART definitions
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
char get_hex_char(unsigned int digit);
int i2c_read_word(unsigned char address, unsigned char reg, unsigned int *data);

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       // Unlock GPIO

    P1DIR |= redLED;
    P1OUT |= redLED;

    Initialize_UART();
    Initialize_I2C();

    unsigned int data;

    uart_write_string("OPT3001 Light Sensor Test\r\n");
    uart_write_string("------------------------\r\n");

    for (;;) {
        // Read Manufacturer ID (Register 0x7E)
        i2c_read_word(0x44, 0x7E, &data);
        uart_write_string("Manufacturer ID: 0x");
        uart_write_uint16(data);
        uart_write_string("\r\n");

        // Read Device ID (Register 0x7F)
        i2c_read_word(0x44, 0x7F, &data);
        uart_write_string("Device ID: 0x");
        uart_write_uint16(data);
        uart_write_string("\r\n\r\n");

        P1OUT ^= redLED;
        __delay_cycles(1000000); // 1-second delay
    }
}

// UART Initialization (eUSCI_A1)
void Initialize_UART(void) {
    P3SEL1 &= ~(BIT4 | BIT5);
    P3SEL0 |=  (BIT4 | BIT5);

    UCA1CTLW0 = UCSWRST;          // Hold in reset
    UCA1CTLW0 |= UCSSEL_2;        // SMCLK
    UCA1BRW = 6;                  // 1 MHz / 9600
    UCA1MCTLW = UCBRF_8 | UCBRS5 | UCOS16;
    UCA1CTLW0 &= ~UCSWRST;        // Release from reset
}

// I2C Initialization (eUSCI_B1)
void Initialize_I2C(void) {
    P4SEL1 |=  (BIT1 | BIT0);    // SDA/P4.1, SCL/P4.0
    P4SEL0 &= ~(BIT1 | BIT0);

    UCB1CTLW0 = UCSWRST;                          // Hold in reset
    UCB1CTLW0 |= UCMST | UCMODE_3 | UCSYNC | UCSSEL_3; // I2C master mode, SMCLK
    UCB1BRW = 8;                                   // 1 MHz / 8 = 125 kHz
    UCB1CTLW0 &= ~UCSWRST;                         // Release from reset
}

// Send a single character over UART
void uart_write_char(unsigned char ch) {
    while (!(FLAGS & TXFLAG));
    TXBUFFER = ch;
}

// Send a null-terminated string over UART
void uart_write_string(char *str) {
    while (*str) {
        uart_write_char(*str++);
    }
}

// Convert a 16-bit unsigned int to 4-digit hex string over UART
void uart_write_uint16(unsigned int n) {
    uart_write_char(get_hex_char((n >> 12) & 0xF));
    uart_write_char(get_hex_char((n >> 8) & 0xF));
    uart_write_char(get_hex_char((n >> 4) & 0xF));
    uart_write_char(get_hex_char(n & 0xF));
}

// Convert integer to hex character
char get_hex_char(unsigned int digit) {
    return (digit < 10) ? ('0' + digit) : ('A' + (digit - 10));
}

// Read a 16-bit word from an I2C register
int i2c_read_word(unsigned char address, unsigned char reg, unsigned int *data) {
    unsigned char msb = 0, lsb = 0;

    UCB1I2CSA = address;                // Set slave address

    // Write register address
    UCB1CTLW0 |= UCTR | UCTXSTT;        // Transmitter + START
    while (!(UCB1IFG & UCTXIFG0));
    UCB1TXBUF = reg;
    while (!(UCB1IFG & UCTXIFG0));

    // Prepare for read
    UCB1CTLW0 &= ~UCTR;                 // Receiver mode
    UCB1CTLW0 |= UCTXSTT;               // Repeated START

    while (!(UCB1IFG & UCRXIFG0));
    msb = UCB1RXBUF;

    UCB1CTLW0 |= UCTXSTP;               // STOP after next byte
    while (!(UCB1IFG & UCRXIFG0));
    lsb = UCB1RXBUF;

    while (UCB1CTLW0 & UCTXSTP);
    while (UCB1STATW & UCBBUSY);

    *data = ((unsigned int)msb << 8) | lsb;
    return 0;
}
