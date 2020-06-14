// Speaker.h
// Juan Paez and Zach Zipper
// TA: Bhagawat Vinay
// Date Created: 2/11/2020
// Last Modified: 2/12/2020
// EE 445L Lab 3
// Runs on TM4C123
// Library for speaker interface.

// Hardware Connections:
// NPN Transistor and Speaker setup connected to Port B0

#include <stdint.h>
#include <stdbool.h>

// Initialize speaker output on PB0
// Inputs:  None
// Outputs:  None
void Speaker_Init(void);

// Simple digital output to PB0
// Inputs:  None
// Outputs:  None
void Speaker_Out(uint8_t data);
