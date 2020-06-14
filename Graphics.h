// Graphics.h
// Juan, Zach, Mithilesh, Jungjoon
// TA: Bhagawat Vinay
// Date Created: 2/11/2020
// Last Modified: 4/27/2020
// EE 445L Lab 11
// Runs on TM4C123
// Library for cocktail mixer graphics.

#include <stdbool.h>
#include <stdint.h>

// Initialize graphics interface by enabling LCD.
// Inputs:  none
// Outputs: none
void Graphics_init(void);

// Draws the Main menu on the ST7735 along with the ratio bar and time
// Inputs: None
// Outputs: None
void Grahpics_DrawMainMenu(void);

// Updates the time and proportions bar on the main menu
// Inputs: uint8_t hours, minutes, and seconds
// Inputs: numerator and denominator for ratio of liquid 1
// Outputs: None
void Graphics_UpdateMainMenu(uint8_t hours, uint8_t minutes, uint8_t seconds, uint32_t num, uint32_t denom);

// *************** Graphics_ProgressDisplayInit ********************
// Used to initialize progress display
// Input: none
// Output: none
void Graphics_ProgressDisplayInit(void);

// *************** Graphics_ProgressDisplayUpdate ********************
// Used to display ratio bar
// Creates a progress bar that is the 160 pixels wide
// Input: currentProgress   current progress of the pour
//        totalProgress     the eventual total progress
// Output: none
void Graphics_ProgressDisplayUpdate(int16_t currentProgress, int16_t totalProgress);
