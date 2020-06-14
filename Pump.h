// Pump.h
// Juan, Zach, Mithilesh, and Jungjoon
// TA: Bhagawat Vinay
// Date Created: 4/10/2020
// Last Modified: 4/10/2020
// EE 445L Lab 11
// Runs on TM4C123
// Library for peristaltic liquid pump interface.

#include <stdint.h>
#include <stdbool.h>

// Initialize liquid pump outputs on PD0 and PD1.
// Inputs:  None
// Outputs:  None
void Pump_Init(void);

// Enable liquid pump on PD0.
// Inputs:  None
// Outputs:  None
void Pump1_On(void);

// Enable liquid pump on PD1.
// Inputs:  None
// Outputs:  None
void Pump2_On(void);

// Disable liquid pump on PD0.
// Inputs:  None
// Outputs:  None
void Pump1_Off(void);

// Enable liquid pump on PD1.
// Inputs:  None
// Outputs:  None
void Pump2_Off(void);
