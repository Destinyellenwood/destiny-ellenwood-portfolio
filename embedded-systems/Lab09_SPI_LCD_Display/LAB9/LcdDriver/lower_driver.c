// This code was ported from TI's sample code. See Copyright notice at the bottom of this file.

#include <LcdDriver/lower_driver.h>
#include "msp430fr6989.h"
#include "Grlib/grlib/grlib.h"
#include <stdint.h>

void HAL_LCD_PortInit(void)
{
    /////////////////////////////////////
    // Configuring the SPI pins
    /////////////////////////////////////

    // Configure UCB0CLK/P1.4 pin to serial clock
    P1SEL0 |= BIT4;
    P1SEL1 &= ~BIT4;


    // Configure UCB0SIMO/P1.6 pin to SIMO
    P1SEL0 |= BIT6;
    P1SEL1 &= ~BIT6;


    // OK to ignore UCB0STE/P1.5 since we'll connect the display's enable bit to low (enabled all the time)
    // OK to ignore UCB0SOMI/P1.7 since the display doesn't give back any data

    ///////////////////////////////////////////////
    // Configuring the display's other pins
    ///////////////////////////////////////////////
    // Set reset pin as output
    P9DIR |= BIT4;
    P9OUT |= BIT4;
    // Set the data/command pin as output
    P2DIR |= BIT3;
    P2OUT &= ~BIT3;
    // Set the chip select pin as output
    P2DIR |= BIT5;
    P2OUT &= ~BIT5;

    return;
}


void HAL_LCD_SpiInit(void)
{
    //////////////////////////
    // SPI configuration
    //////////////////////////

    // Put eUSCI in reset state and set all fields in the register to 0
    UCB0CTLW0 = UCSWRST;

    // Fields that need to be nonzero are changed below

    // Set clock phase to "capture on 1st edge, change on following edge"
    UCB0CTLW0 |= UCCKPH; // This is the clock phase select. Set to 1b from table.

    // Set clock polarity to "inactive low"
    UCB0CTLW0 &= ~UCCKPL; // This is the clock polarity select. Set to 0b for inactive state is low from table.

    // Set data order to "transmit MSB first"
    UCB0CTLW0 |= UCMSB; // This is the MSB first select. Set to 1b for MSB select. This controls the direction of the receive and transmit shift register.

    // Set data size to 8-bit
    UCB0CTLW0 &= ~UC7BIT; // This is the Character length. Set to 0b for 8-bit data from table.

    // Set MCU to "SPI master"
    UCB0CTLW0 |= UCMST; // This is the Master mode select. Set to 1b for Master mode from table.

    // Set SPI to "3-pin SPI" (we won't use eUSCI's chip select)
    UCB0CTLW0 |= UCMODE_0; // This eUSCI mode. The UCMODEx bits select the synchronous mode when UCSYNC = 1. Set to 00b for 3-pin SPI from table.

    // Set module to synchronous mode
    UCB0CTLW0 |= UCSYNC; // This is the Synchronous mode enable. Set to 1b from table.

    // Set clock to SMCLK
    UCB0CTLW0 |= UCSSEL_2; // This is the eUSCI clock source select. These bits select the BRCLK source clock. Set to 10b for SMCLK in master mode, don't use in slave mode from table.

    // Configure the clock divider (SMCLK is set to 16 MHz; run SPI at 8 MHz using SMCLK)
    // Maximum SPI supported frequency on the display is 10 MHz
    // Using the formula: fbitclock = fclock / UCBRx
    // 8 MHz = 16 MHz / UCBRx
    // UCBRx = 16 MHz / 8 MHz = 2
    UCB0BRW = 2; // Set SPI clock divider to 2 for 8MHz SPI frequency with 16 MHz SMCLK.

    // Exit the reset state at the end of the configuration
    UCB0CTLW0 &= ~UCSWRST; // This is the software reset enable. Set to 0b for disabled. eUSCI reset released for operation.

    // Set CS' (chip select) bit to 0 (display always enabled)
    P1DIR |= BIT3; // Set P1.3 as output (CS pin)
    P1OUT &= ~BIT3; // Set P1.3 to low (display always enabled)
    P1SEL0 &= ~BIT3;  // Set P1SEL0 to 0 for GPIO
    P1SEL1 &= ~BIT3; // Set P1SEL1 to 0 for GPIO

    // Set DC' bit to 0 (assume data)
    P1DIR |= BIT2; // Set P1.2 as output (DC pin)
    P1OUT &= ~BIT2; // Set P1.2 to low (default to command mode)
    P1SEL0 &= ~BIT2;  // Set P1SEL0 to 0 for GPIO
    P1SEL1 &= ~BIT2; // Set P1SEL1 to 0 for GPIO
    //*/

    return;
}


//*****************************************************************************
// Writes a command to the CFAF128128B-0145T.  This function implements the basic SPI
// interface to the LCD display.
//*****************************************************************************
void HAL_LCD_writeCommand(uint8_t command)
{
    // Wait as long as the module is busy
    while (UCB0STATW & UCBUSY);

    // For command, set the DC' bit to low before transmission
    P2OUT &= ~BIT3;

    // Transmit data
    UCB0TXBUF = command;

    return;
}


//*****************************************************************************
// Writes a data to the CFAF128128B-0145T.  This function implements the basic SPI
// interface to the LCD display.
//*****************************************************************************
void HAL_LCD_writeData(uint8_t data)
{
    // Wait as long as the module is busy
    while (UCB0STATW & UCBUSY);

    // Set DC' bit back to high
    P2OUT |= BIT3;

    // Transmit data
    UCB0TXBUF = data;

    return;
}






/* --COPYRIGHT--,BSD
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
