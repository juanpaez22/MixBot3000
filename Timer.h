// Timer.h
// Juan Paez and Zach Zipper
// TA: Bhagawat Vinay
// Date Created: 2/11/2020
// Last Modified: 4/27/2020
// EE 445L Lab 11
// Runs on TM4C123
// This software configures the timer to store current time
// Makes use of Timer0
// Requires Timer.h
// Port F Must be initialized!

#include <stdint.h>

extern volatile uint8_t Timer_Seconds, Timer_Minutes, Timer_Hours;

// Initializes timer2a to 60 hz and enables its interrupts
// Inputs: None
// Outputs: None
void Timer_Init(void);

// Sets global time.
// Input: uint8_t for seconds, minutes, and hours
// Output: 1 if valid input, 0 otherwise
uint8_t Timer_Set_Time(uint8_t sec, uint8_t min, uint8_t hour);

uint8_t Timer_Get_Hours(void);
uint8_t Timer_Get_Minutes(void);
uint8_t Timer_Get_Seconds(void);
