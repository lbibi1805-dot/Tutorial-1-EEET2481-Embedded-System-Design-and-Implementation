/*************************************************************************//**
 * @file     main.c
 * @version  V1.00 
 * @brief    EEET2480 - TUTORIAL 1 - LED SWEEP
 * @board: NuMaker-PFM-M487
 * Description: This program will sweep through 3 LEDs on the board with a delay. 
 * LED0, LED1, and LED2 are connected to PH.0, PH.1, and PH.2, respectively.
*****************************************************************************/
#include <stdio.h>
#include "NuMicro.h"

#define HXTSTB (1<<0) // HXT Clock Source Stable Flag
#define PLLSTB (1<<2) // Internal PLL Clock Source Stable Flag
#define HXTEN  (1<<0) // HXT Enable Bit
#define LED0 (1<<0)   // PH.0
#define LED1 (1<<1)   // PH.1
#define LED2 (1<<2)   // PH.2

void SYS_Init(void) {
    SYS_UnlockReg();

    // Set XT1_OUT (PF.2) and XT1_IN (PF.3) to input mode
    PF->MODE &= ~((0x3 << 4) | (0x3 << 6));

    // Enable HXT
    CLK->PWRCTL |= HXTEN;
    while (!(CLK->STATUS & HXTSTB));

    // Configure PLL for 192 MHz (FIN=12 MHz, NR=2, NF=64, NO=2)
    CLK->PLLCTL = (1 << 8) | (64 << 9) | (1 << 16); // INDIV=1, FBDIV=64, OUTDIV=01
    while (!(CLK->STATUS & PLLSTB));

    // Set HCLK to PLLFOUT
    CLK->CLKSEL0 &= ~(0x7 << 0);
    CLK->CLKSEL0 |= 0x2;

    // Set HCLK Divider to 0
    CLK->CLKDIV0 &= ~0xF;
    CLK->CLKDIV0 |= 0x0;

    // Set PH.0 ~ PH.2 as GPIO
    SYS->GPH_MFPL &= ~((0xF << 0) | (0xF << 4) | (0xF << 8));
    SYS->GPH_MFPL |= ((0x0 << 0) | (0x0 << 4) | (0x0 << 8));

    SYS_LockReg();
}

int main() {
    SYS_Init();

    // Set PH.0, PH.1, PH.2 as output push-pull
    PH->MODE &= ~((0x3 << 0) | (0x3 << 2) | (0x3 << 4));
    PH->MODE |= ((0x1 << 0) | (0x1 << 2) | (0x1 << 4));

    /* Old code: LED sweep
    while(1) {
        PH->DOUT ^= (1<<0);
        CLK_SysTickDelay(10000000);
        PH->DOUT ^= (1<<1);
        CLK_SysTickDelay(10000000);
        PH->DOUT ^= (1<<2);
        CLK_SysTickDelay(10000000);
    }
    */

    // Turn on all LEDs (assuming active-low)
    PH->DOUT &= ~(LED0 | LED1 | LED2); // Set PH.0, PH.1, PH.2 to 0

    while(1); // Keep program running
}

