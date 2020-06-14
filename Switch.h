// Switch.h
// Juan Paez and Zach Zipper
// TA: Bhagawat Vinay
// Date Created: 2/11/2020
// Last Modified: 4/10/2020
// EE 445L Lab 11
// Runs on TM4C123
// Library for simple switch and pot control interface.

// 10kOhm potentiometer at PD3 ADC
// Switches at PB1 and PB2 (positive logic).

#include <stdbool.h>

// Initialize control interface by enabling ADC and initializing Port F switches
// Inputs:  none
// Outputs: none
void Switch_init(void);

// Get the ADC value supplied by the potentiometer.
// Inputs:  none
// Outputs: integer from 0 to 4095 representing ADC value.
int Switch_get_pot_ADC(void);

// Get whetehr switch at PB1 is down.
// Inputs:	none
// Outputs:	whether sw1 is pressed
int Switch_get_sw1_down(void);

// Get whetehr switch at PB2 is down.
// Inputs:	none
// Outputs:	whether sw1 is pressed
int Switch_get_sw2_down(void);
