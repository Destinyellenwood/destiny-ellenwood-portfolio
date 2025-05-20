// Lab 09 – SPI LCD Display with Screen Switching and Counter
// Destiny Ellenwood – MSP430FR6989

#include <msp430.h>
#include "grlib.h"
#include "Crystalfontz128x128_HW_SPI.h"
#include "images/logo.h"

#define redLED    BIT0
#define S1        BIT1

volatile unsigned char current_screen = 0;
volatile unsigned int counter = 0;

void HAL_LCD_PortInit(void);
void HAL_LCD_SpiInit(void);
void Initialize_TimerA(void);
void Draw_Snoopy_Logo(Graphics_Context *context);
void Draw_Text_And_Shapes_Screen(Graphics_Context *context);

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= redLED;
    P1OUT &= ~redLED;

    // Button S1 setup
    P1DIR &= ~S1;
    P1REN |= S1;
    P1OUT |= S1;
    P1IES |= S1;
    P1IFG &= ~S1;
    P1IE |= S1;

    HAL_LCD_PortInit();
    HAL_LCD_SpiInit();
    Crystalfontz128x128_Init();

    // Graphics context
    Graphics_Context g_context;
    Graphics_initContext(&g_context, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_context, GRAPHICS_COLOR_WHITE);
    Graphics_setBackgroundColor(&g_context, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_context, &g_sFontFixed6x8);
    Graphics_clearDisplay(&g_context);

    Draw_Snoopy_Logo(&g_context);
    Initialize_TimerA();
    __enable_interrupt();

    while (1) {
        P1OUT ^= redLED;

        if (current_screen == 1) {
            Draw_Text_And_Shapes_Screen(&g_context);
        }

        __delay_cycles(500000);
    }
}

// ISR for Button S1
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
    if (P1IFG & S1) {
        current_screen ^= 1;
        counter = 0;
        Graphics_clearDisplay(&g_sContext);
        P1IFG &= ~S1;
    }
}

// ISR for Timer A0 – update counter every 100 ms
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A_ISR(void) {
    if (current_screen == 1) {
        counter++;
    }
}

// Draw static logo image
void Draw_Snoopy_Logo(Graphics_Context *context) {
    Graphics_drawImage(context, &logo, 0, 0);
}

// Draw text and shapes screen with counter
void Draw_Text_And_Shapes_Screen(Graphics_Context *context) {
    Graphics_clearDisplay(context);

    Graphics_setForegroundColor(context, GRAPHICS_COLOR_GREEN);
    Graphics_drawLineH(context, 10, 118, 10);
    Graphics_drawLineV(context, 10, 10, 118);
    Graphics_drawLineH(context, 10, 118, 118);
    Graphics_drawLineV(context, 118, 10, 118);

    Graphics_setForegroundColor(context, GRAPHICS_COLOR_YELLOW);
    Graphics_drawCircle(context, 64, 64, 20);

    Graphics_setForegroundColor(context, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(context, "UCF Knight", AUTO_STRING_LENGTH, 40, 50, OPAQUE_TEXT);

    char buffer[10];
    sprintf(buffer, "%u", counter);
    Graphics_drawString(context, buffer, AUTO_STRING_LENGTH, 55, 90, OPAQUE_TEXT);
}

// Configure SPI pins
void HAL_LCD_PortInit(void) {
    P1SEL0 |= BIT4 | BIT6;   // CLK (P1.4), MOSI (P1.6)
    P1SEL1 &= ~(BIT4 | BIT6);

    P1DIR |= BIT2 | BIT3;    // DC (P1.2), CS (P1.3)
    P1OUT |= BIT2 | BIT3;
}

// Initialize SPI for LCD (UCB0)
void HAL_LCD_SpiInit(void) {
    UCB0CTLW0 = UCSWRST;
    UCB0CTLW0 |= UCSSEL__SMCLK | UCMST | UCSYNC | UCCKPH | UCMSB;
    UCB0BRW = 1;
    UCB0CTLW0 &= ~UCSWRST;
    UCB0IE &= ~UCRXIE;
}

// Timer A0 init for 100 ms intervals
void Initialize_TimerA(void) {
    TA0CCTL0 = CCIE;
    TA0CCR0 = 3276;  // 100ms @ 32.768 kHz
    TA0CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;
}
