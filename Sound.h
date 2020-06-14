// Sound.h
// Juan, Zach, Mithilesh, Jungjoon
// TA: Bhagawat Vinay
// Date Created: 2/11/2020
// Last Modified: 4/27/2020
// EE 445L Lab 11
// Runs on TM4C123
// Library for sound output interface.
// Hardware Connections:
// NPN Transistor and Speaker setup

#include <stdint.h>

// Initialize sound interface and interrupts.
// Inputs:  None
// Outputs:  None
void Sound_Init(void);

// Stop sound play by disabling interrupts.
// Requires SysTick and Timer1A
// Inputs:  None
// Outputs:  None
void Sound_Stop(void);

// Play short sound on startup
// Input: None
// Output: None
void Sound_StartUp(void);

// Play short sound on drink completion
// Input: None
// Output: None
void Sound_Done(void);
