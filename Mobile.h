// Mobile.h
// Juan, Zach, Mithilesh, and Jungjoon
// TA: Bhagawat Vinay
// Date Created: 4/10/2020
// Last Modified: 4/27/2020
// EE 445L Lab 11
// Runs on TM4C123
// Library for mobile interface for cocktail maker.

// Initialize WiFi communication with ESP and set up virtual pins.
// Also gets current time from an API.
// Input:		Returns time by populating hour, minute, and second pointers.
// Output:	None
void Mobile_init(int* hour, int* minute, int* second);

// Get whether virtual switch 1 is down or not.
// Input:		None
// Output:	1 if sw1 is down, 0 otherwise.
int Mobile_get_sw1_down(void);

// Get whether virtual switch 2 is down or not.
// Input:		None
// Output:	1 if sw1 is down, 0 otherwise.
int Mobile_get_sw2_down(void);

// Get whether virtual switch 1 is down or not.
// Input:		None
// Output:	1 if sw1 is down, 0 otherwise.
int Mobile_get_slider(void);

// Send a value from 0 to 100 representing progress bar.
// Input:		percent- integer from 0 to 100.
// Output:	None
void Mobile_send_progress(int percent);
