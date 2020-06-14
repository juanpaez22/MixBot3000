// Pump.c
// Juan, Zach, Mithilesh, and Jungjoon
// TA: Bhagawat Vinay
// Date Created: 4/10/2020
// Last Modified: 4/10/2020
// EE 445L Lab 11
// Runs on TM4C123
// Library for peristaltic liquid pump interface.

#include "Pump.h"
#include "../inc/tm4c123gh6pm.h"

// Initialize liquid pump outputs on PD0 and PD1.
// Inputs:  None
// Outputs:  None
void Pump_Init(void) {
	SYSCTL_RCGCGPIO_R |= 0x08;
  while((SYSCTL_PRGPIO_R&0x08) != 0x08){};
	GPIO_PORTD_AMSEL_R &= ~0x01;      // no analog
	GPIO_PORTD_PCTL_R &= ~0x00000FFF; // regular GPIO function
  GPIO_PORTD_DIR_R |= 0x03;
  GPIO_PORTD_AFSEL_R &= ~0x03;
	GPIO_PORTD_DEN_R |= 0x03;
}

// Enable liquid pump on PD0.
// Inputs:  None
// Outputs:  None
void Pump1_On(void) {
	GPIO_PORTD_DATA_R |= 0x01;
}

// Enable liquid pump on PD1.
// Inputs:  None
// Outputs:  None
void Pump2_On(void) {
	GPIO_PORTD_DATA_R |= 0x02;
}

// Disable liquid pump on PD0.
// Inputs:  None
// Outputs:  None
void Pump1_Off(void) {
	GPIO_PORTD_DATA_R &= ~(0x01);
}

// Enable liquid pump on PD1.
// Inputs:  None
// Outputs:  None
void Pump2_Off(void) {
	GPIO_PORTD_DATA_R &= ~(0x02);
}
