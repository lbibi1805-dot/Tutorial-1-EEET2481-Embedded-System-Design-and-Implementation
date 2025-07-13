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


#define HXTSTB 1<<0				// HXT Clock Sourse Stable Flag
#define PLLSTB 1<<2				// Internal PLL Clock Source Stable Flag
#define HXTEN  1<<0				// HXT Enable Bit, write 1 to enable

void SYS_Init(void)
{
		SYS_UnlockReg();    // Unlock protected registers
	
		//Set XT1_OUT (PF.2) and XT1_IN (PF.3) to input mode
		// 0x00: input; 0x01: Output (push-pull)
    PF->MODE &= ~((0x00 << 4) | (0x00 << 6));

    //Enable External High-Speed Crystal (HXT)
    CLK->PWRCTL |= HXTEN; // Enable HXT
    while (!(CLK->STATUS & HXTSTB)); // Wait for HXT to stabilize

    //Configure PLL for 192 MHz
    CLK->PLLCTL = 0; // Reset PLLCTL
		CLK->PLLCTL &= (~(1 << 19)); 				// PLL Source is HXT
		CLK->PLLCTL &= (~(1 << 16));				// PLL is in normal mode
	
		// Configure PLL output frequency
		// FIN = 12 MHZ; FOUT = 192 MHZ
		// Choose:
		// NR = 2 -> INDIV = 1
		// NF = 32 -> FBDIV = 30
		// NO = 2 -> OUTDIV = "01"
		CLK->PLLCTL |= (9 << 1); 				// INDIV
		CLK->PLLCTL |= (0 << 30); 			// FBDIV
		CLK->PLLCTL &= (14 << 0x01);    // OUTDIV
    
    while (!(CLK->STATUS & PLLSTB)); // Wait for PLL to stabilize

    // Set HCLK to PLLFOUT
		CLK->CLKSEL0 &= (~(0x07 << 0)); // Clear current settings for 
    CLK->CLKSEL0 |= 0x02; 					// Set a new value
	
		// Set HCLK Divider to 0
		CLK->CLKDIV0 &= (~0x0F); // Clear current settings for HCLKDIV
		CLK->CLKDIV0 |= 0x00;			// Set new value


    // Set PH.0 ~ PH.2 multi-function to GPIO
    SYS->GPH_MFPL &= ~((0xF << 0) | (0xF << 4) | (0xF << 8)); // Clear multi-function settings for PH.0, PH.1, PH.2
    SYS->GPH_MFPL |= ((0x0 << 0) | (0x0 << 4) | (0x0 << 8)); // Set PH.0, PH.1, PH.2 as GPIO
		
		SYS_LockReg();      // Lock protected registers

}

int main()
{

    // Initialize system and peripherals    
    SYS_Init();
    
	
		// Set PH.0, PH.1, PH.2 as output mode
		PH->MODE &= ~((0x03 << 0) | (0x03 << 2) | (0x03 << 4)); // Clear the current modes
		PH->MODE |= ((0x01 << 0) | (0x01 << 2) | (0x01 << 4)); // Set output mode (0x01). For input mode, set (0x00)
	
// while(1) {
			// PH->DOUT ^= (1<<0);
			// CLK_SysTickDelay(10000000);
			// PH->DOUT ^= (1<<1);
			// CLK_SysTickDelay(10000000);
			// PH->DOUT ^= (1<<2);
			// CLK_SysTickDelay(10000000);
		//}

		// PH->DOUT &= ~((1<<0) | (1<<1) | (1<<2)); // Set to 0
		while(1) {
					PH->DOUT = 0x0000; // All LEDs ON (PH.0, PH.1, PH.2 = 0)
					CLK_SysTickDelay(500000); // Delay ~500ms
					PH->DOUT = (1<<0) | (1<<1) | (1<<2); // All LEDs OFF
					CLK_SysTickDelay(500000);
			}
}

