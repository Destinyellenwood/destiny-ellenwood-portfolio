// Lab 5.3 – Chronometer Application
// Destiny Ellenwood – MSP430FR6989

#include <msp430fr6989.h>

#define redLED    BIT0
#define greenLED  BIT7
#define S1        BIT1
#define S2        BIT2

volatile unsigned long time_seconds = 0;
volatile int run_stop = 0;
volatile int colon_state = 0;

void Initialize_LCD();
void lcd_write_time(unsigned long seconds);

const unsigned char LCD_Shapes[10] = {
    0xFC, 0x60, 0xDB, 0xF1, 0x67,
    0xB7, 0xBF, 0xE0, 0xFF, 0xF7
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
        __low_power_mode_3(); // Sleep until next event
    }
}

// Timer ISR – triggers every 1 second
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A0_ISR(void) {
    if (run_stop) {
        time_seconds++;
        if (time_seconds >= 86400UL)
            time_seconds = 0;

        colon_state = !colon_state;
    }

    lcd_write_time(time_seconds);

    // UI: colon, chronometer, exclamation
    if (run_stop) {
        LCDM7 = colon_state ? (LCDM7 | 0x04) : (LCDM7 & ~0x04); // Blink colon
        LCDM3 |= 0x08;  // Chronometer icon
        LCDM3 &= ~0x01; // Exclamation off
    } else {
        LCDM7 |= 0x04;   // Solid colon
        LCDM3 &= ~0x08;  // Chronometer off
        LCDM3 |= 0x01;   // Exclamation on
    }

    LCDM20 |= 0x01; // Turn on DP
    TA0CCTL0 &= ~CCIFG;
    __low_power_mode_off_on_exit();
}

// Port 1 ISR – handles S1 and S2
#pragma vector = PORT1_VECTOR
__interrupt void Port_1_ISR(void) {
    if (P1IFG & S2) {
        if (!(P1IN & S2)) {
            __delay_cycles(1000000);
            while (!(P1IN & S2)) {
                if (P1IN & S1) {
                    time_seconds++;
                    if (time_seconds >= 86400UL) time_seconds = 0;
                } else {
                    if (time_seconds > 0)
                        time_seconds--;
                    else
                        time_seconds = 86399UL;
                }
                lcd_write_time(time_seconds);
                __delay_cycles(100000);
            }
        }
    }

    if (P1IFG & S1) {
        if (!(P1IN & S1)) {
            __delay_cycles(1000000);
            if (!(P1IN & S1)) {
                time_seconds = 0;
            } else {
                run_stop = !run_stop;
            }
            lcd_write_time(time_seconds);
        }
    }

    __delay_cycles(20000);
    P1IFG &= ~(S1 | S2);
    __low_power_mode_off_on_exit();
}

// Display time in HH:MM:SS format
void lcd_write_time(unsigned long seconds) {
    unsigned int hrs = seconds / 3600;
    unsigned int mins = (seconds % 3600) / 60;
    unsigned int secs = seconds % 60;

    volatile unsigned char* const LCDM_map[6] = {
        &LCDM8, &LCDM15, &LCDM19, &LCDM4, &LCDM6, &LCDM10
    };

    unsigned int time_array[3] = {secs, mins, hrs};

    for (int i = 0; i < 3; i++) {
        unsigned int value = time_array[i];
        *LCDM_map[i*2]     = LCD_Shapes[value % 10];
        *LCDM_map[i*2 + 1] = LCD_Shapes[value / 10];
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
