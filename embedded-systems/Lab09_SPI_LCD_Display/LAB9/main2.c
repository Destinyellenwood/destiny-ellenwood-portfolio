
/*
EEL 4742C - UCF

Code that alternates between two screens and displays a logo and static text with shapes.
*/

#include "msp430fr6989.h"
#include "Grlib/grlib/grlib.h"          // Graphics library (grlib)
#include "LcdDriver/lcd_driver.h"       // LCD driver
#include <stdio.h>
#include <stdbool.h>                    // Ensure bool type is recognized

#define redLED BIT0
#define greenLED BIT7
#define S1 BIT1

volatile unsigned int counter = 0;

void Initialize_Clock_System();
void Draw_Snoopy_Logo(Graphics_Context *g_sContext);
void Draw_Text_And_Shapes_Screen(Graphics_Context *g_sContext, bool firstDraw, int counter);
void Initialize_TimerA();



void main(void) {
    int screen = 0;  // Variable to track the current screen (0 = Snoopy Logo, 1 = Text and Shapes Screen)
    bool firstDraw = true; // Flag to indicate if the screen is being drawn for the first time


    // Configure WDT & GPIO
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    // Configure LEDs
    P1DIR |= redLED; // Set the red LED as output
    P9DIR |= greenLED; // Set the green LED as output
    P1OUT &= ~redLED; // Turn off red LED
    P9OUT &= ~greenLED; // Turn off red LED

    // Configure button S1
    P1DIR &= ~S1; // Set button S1 as input
    P1REN |= S1; // Enable pull-up/pull-down resistor for S1
    P1OUT |= S1; // Set resistor to pull-up mode

    // Configure clock system
    Initialize_Clock_System();

    // Initialize TimerA for counter updates
    Initialize_TimerA();

    // Enable global interrupts for Timer A
    __enable_interrupt();

    // Initialize LCD and graphics context
    Graphics_Context g_sContext;
    Crystalfontz128x128_Init(); // Initialize LCD
    Crystalfontz128x128_SetOrientation(0); // Set LCD orientation
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128); // Initialize graphics context

    // Initial Screen (Snoopy Logo)
    Draw_Snoopy_Logo(&g_sContext);

    // Main loop
    while (1) {
        // Check if button S1 is pressed to toggle screens
        if (!(P1IN & S1)) { // Button S1 pressed (active low)
            __delay_cycles(3000000); // Delay
            screen = !screen;       // Toggle between screens
            firstDraw = true;       // Indicate a fresh draw for the new screen
            counter = -1;           // Reset counter to start from 0 on next update

            if (screen == 0) {
                TA0CTL &= ~MC_3; // Stop Timer A when showing the logo screen
                Draw_Snoopy_Logo(&g_sContext); // Display Snoopy Logo

            } else {
                Initialize_TimerA(); // Start Timer A when on text and shapes screen
                Draw_Text_And_Shapes_Screen(&g_sContext, firstDraw, counter); // Draw shapes and counter
                firstDraw = false; // Clear firstDraw flag after initial draw
            }
        }
        // If shapes on screen, update the counter
        if (screen == 1) {
           Draw_Text_And_Shapes_Screen(&g_sContext, false, counter); // Update only the counter
           __delay_cycles(3000000);   // Delay for counter update speed
        }
        P1OUT ^= redLED; // Toggle red LED to show activity
    }
}

void Initialize_Clock_System() {
  // DCO frequency = 16 MHz
  // MCLK = fDCO/1 = 16 MHz
  // SMCLK = fDCO/1 = 16 MHz

  // Activate memory wait state
  FRCTL0 = FRCTLPW | NWAITS_1;    // Wait state=1
  CSCTL0 = CSKEY;
  // Set DCOFSEL to 4 (3-bit field)
  CSCTL1 &= ~DCOFSEL_7;
  CSCTL1 |= DCOFSEL_4;
  // Set DCORSEL to 1 (1-bit field)
  CSCTL1 |= DCORSEL;
  // Change the dividers to 0 (div by 1)
  CSCTL3 &= ~(DIVS2|DIVS1|DIVS0);    // DIVS=0 (3-bit)
  CSCTL3 &= ~(DIVM2|DIVM1|DIVM0);    // DIVM=0 (3-bit)
  CSCTL0_H = 0;

  return;
}

// Function to display the Snoopy Logo on the screen
void Draw_Snoopy_Logo(Graphics_Context *g_sContext) {
    extern const Graphics_Image SNOOPY4BPP_UNCOMP;  // Use the external Snoopy image
    Graphics_clearDisplay(g_sContext); //  Clear display
    Graphics_drawImage(g_sContext, &SNOOPY4BPP_UNCOMP, 0, 0);  // Draw Snoopy Logo at (0,0)
}

// Function to display text, shapes, and counter on the screen
void Draw_Text_And_Shapes_Screen(Graphics_Context *g_sContext, bool firstDraw, int counter) {
    char mystring[20];

    if (firstDraw) {
        // Set the background color and clear display once
        Graphics_setBackgroundColor(g_sContext, GRAPHICS_COLOR_VIOLET);
        Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_BLACK);

        GrContextFontSet(g_sContext, &g_sFontcourier7x13); // Set first font for text

        Graphics_clearDisplay(g_sContext); // Clear disaply for fresh start

        // Display static text
        Graphics_drawStringCentered(g_sContext, "EEL 4742", AUTO_STRING_LENGTH, 64, 10, OPAQUE_TEXT);
        GrContextFontSet(g_sContext, &g_sFontlucidasans6x11); // Display second font for text
        sprintf(mystring, "Destiny's Demo!"); // Prepare demo text
        Graphics_drawStringCentered(g_sContext, (int8_t *)mystring, AUTO_STRING_LENGTH, 64, 20, OPAQUE_TEXT);

        // Draw a horizontal line
        Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawLineH(g_sContext, 20, 108, 40);

        // Draw a filled circle
        Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_HOT_PINK);
        Graphics_fillCircle(g_sContext, 30, 80, 5);

        // Draw a filled rectangle in gold color
        Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_GOLD);
        Graphics_fillRectangle(g_sContext, &(Graphics_Rectangle){50, 75, 60, 85});

        // Draw an outlined circle
        Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_PAPAYA_WHIP);
        Graphics_drawCircle(g_sContext, 98, 80, 5);

        // Draw a rectangle outline at the bottom
        Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_DARK_OLIVE_GREEN);
        Graphics_drawRectangle(g_sContext, &(Graphics_Rectangle){30, 95, 98, 105});

        // Draw "ctr" label
        Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawString(g_sContext, "ctr", AUTO_STRING_LENGTH, 90, 50, OPAQUE_TEXT);
    }

    // Clear previous counter display area
    Graphics_Rectangle clearArea = {50, 48, 70, 60}; // Define area to clear for counter update
    Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_VIOLET); // Set to background color
    Graphics_fillRectangle(g_sContext, &clearArea); // Clear the area

    // Display the counter
    sprintf(mystring, "%d", counter); // Convert counter to string
    Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_BLACK); // Set text color
    Graphics_drawStringCentered(g_sContext, (int8_t *)mystring, AUTO_STRING_LENGTH, 60, 54, OPAQUE_TEXT);
}

// Timer A counts up to 3270 using the ACLK with interrupts enabled.
void Initialize_TimerA(void) {
    TA0CCR0 = 3270; // Set CCR0 value for a 100ms interval with ACLK at 32768 Hz
    TA0CCTL0 &= ~CCIFG;  // Clear the capture/compare interrupt flag
    TA0CCTL0 |= CCIE; // Enable interrupt for CCR0
    TA0CTL = TASSEL_1 | ID_0 | MC_1 | TACLR; // ACLK with a divider 1, Up mode where timer counts up to 3270 then resets to 0
    TA0CTL &= ~TAIFG; // Clear the interrupt flag
}



#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0_ISR(void) {
    counter++;                 // Increment the counter
    if (counter > 255) {
        counter = 0;
    }
    // Clear the interrupt flag
    TA0CCTL0 &= ~CCIFG;
}

