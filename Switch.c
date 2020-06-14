// Switch.c
// Juan, Zach, Mithilesh, Jungjoon
// TA: Bhagawat Vinay
// Date Created: 2/11/2020
// Last Modified: 4/10/2020
// EE 445L Lab 11
// Runs on TM4C123
// Library for simple switch and pot control interface.

#include <stdbool.h>
#include "Switch.h"
#include "../inc/LaunchPad.h"
#include "../inc/ADCSWTrigger.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/CortexM.h"

// Initialize control interface by enabling ADC and initializing Port F switches
// Inputs:  none
// Outputs: none
void Switch_init(void) {
  ADC0_InitSWTriggerSeq3(4);     // initialize ADC at PD3
  ADC0_SAC_R &= 0xFFFFFFF8;
  ADC0_SAC_R |= 0x04;                // Some hardware averaging.
	
	SYSCTL_RCGCGPIO_R |= 0x02;
  while((SYSCTL_PRGPIO_R&0x02) != 0x02){};
	GPIO_PORTB_AMSEL_R &= ~0x06;      // no analog
  GPIO_PORTB_PCTL_R &= ~0x00000FFF; // regular GPIO function
  GPIO_PORTB_DIR_R &= ~0x06;
  GPIO_PORTB_AFSEL_R &= ~0x06;   // disable alt funct on PB1 and 2
	GPIO_PORTB_DEN_R |= 0x06;      // enable digital I/O on PB1 and PB2
}

// Get the ADC value supplied by the potentiometer.
// Inputs:  none
// Outputs: integer from 0 to 100 representing ADC percentage.
int Switch_get_pot_ADC(void) {
  int val = ADC0_InSeq3();
	if (val < 100) val = 0;
	else if (val > 4000) val = 100;
	else val = (val - 100) / 39;
	return val;
}

// Get whetehr switch at PB1 is down.
// Inputs:	none
// Outputs:	whether sw1 is pressed
int Switch_get_sw1_down(void) {
	return (GPIO_PORTB_DATA_R & 0x02) != 0;
}

// Get whetehr switch at PB2 is down.
// Inputs:	none
// Outputs:	whether sw1 is pressed
int Switch_get_sw2_down(void) {
	return (GPIO_PORTB_DATA_R & 0x04) != 0;
}
