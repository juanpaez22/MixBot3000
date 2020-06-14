// Speaker.c
// Juan Paez and Zach Zipper
// TA: Bhagawat Vinay
// Date Created: 2/11/2020
// Last Modified: 2/12/2020
// EE 445L Lab 3
// Runs on TM4C123
// Library for speaker interface.

#include "Speaker.h"
#include "../inc/tm4c123gh6pm.h"
#define PB0 (*((volatile uint32_t *)0x40005004))

// Initialize speaker output on PB0
// Inputs:  None
// Outputs:  None
void Speaker_Init(void){
  SYSCTL_RCGCGPIO_R |= 0x02; // activate port B
  while( (SYSCTL_RCGCGPIO_R&0x02)==0){};    // allow time to finish activating
  GPIO_PORTB_AMSEL_R &= ~0x01;      // no analog
  GPIO_PORTB_PCTL_R &= ~0x00000FFF; // regular GPIO function
  GPIO_PORTB_DIR_R |= 0x01;      // make PB0 out
  GPIO_PORTB_AFSEL_R &= ~0x01;   // disable alt funct on PB0
  GPIO_PORTB_DEN_R |= 0x01;      // enable digital I/O on PB0
}

// Simple digital output to PB0
// Inputs:  None
// Outputs:  None
void Speaker_Out(uint8_t data){
  PB0 = data;
}
