// Lab11.c
// Modified by Juan, Zach, Mithilesh, and Jungjoon
// TA: Bhagawat Vinay
// Last Modified: 4/13/2020 for EE445L Lab 11: MixBot3000
// Runs on TM4C123
// This program runs on the TM4C123 to drive a cocktail mixer.
// Author: Juan, Zach, Mithilesh, and Jungjoon

// Hardware described in included SCH file.

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/PLL.h"
#include "../inc/LaunchPad.h"
#include "../inc/CortexM.h"
#include "ST7735.h"
#include "Graphics.h"
#include "Sound.h"
#include "Switch.h"
#include "Timer.h"
#include "Pump.h"
#include "Mobile.h"

// Initial values and globals.
bool mobile_mode = false;

// Delay 10 ms for debouncing and debugging
// Inputs:  none
// Outputs:  none
void DelayWait10ms(uint32_t n){
  uint32_t volatile time;
  while(n){
    time = 727240*2/91;  // 10msec
    while(time){
      time--;
    }
    n--;
  }
}

// Wait for PF4 switch press and release, with debouncing
// Inputs:  none
// Outputs:  none
void Pause(void){
  while(PF4==0x00){ 
    DelayWait10ms(10);
  }
  while(PF4==0x10){
    DelayWait10ms(10);
  }
}

// Helper function to turn on pumps to pour desired proportions.
// Input:		Proportion of second drink, int from 0 to 100.
// Output:	None
void PourDrink(int proportion) {
	Graphics_ProgressDisplayInit();
	int total_time = 116;
	int time_pump_2 = proportion * total_time / 100;
	int time_pump_1 = total_time - time_pump_2;
	int max_time = time_pump_1 > time_pump_2 ? time_pump_1 : time_pump_2;
	
	int time_elapsed = 0;
	int prev_sec = Timer_Seconds;
	
	if (time_pump_1 > 0) Pump1_On();
	if (time_pump_2 > 0) Pump2_On();
	
	while(time_elapsed < max_time) {
		if (Timer_Seconds == prev_sec) continue;
		prev_sec = Timer_Seconds;
		Graphics_ProgressDisplayUpdate(time_elapsed * 100 / max_time, 200);
		Mobile_send_progress(time_elapsed * 100 / max_time);
		time_elapsed++;
		if (time_elapsed >= time_pump_1) Pump1_Off();
		if (time_elapsed >= time_pump_2) Pump2_Off();
	}
	Pump1_Off();	// just in case
	Pump2_Off();
	DelayWait10ms(1000);	// a bit to drip
	Mobile_send_progress(0);
}

int main() {
	PLL_Init(Bus80MHz);   // Bus clock at 80 MHz
	DisableInterrupts();  // Disable interrupts until finished with inits
	
	// Low level driver initializations
  LaunchPad_Init();
	Sound_Init();
	Switch_init();
	Graphics_init();
  Pump_Init();
	Timer_Init();
	
	// Mobile initialization and real-time fetch
	int init_hour = 0, init_min = 0, init_sec = 0;
	Mobile_init(&init_hour, &init_min, &init_sec); //comment out if using simulator
	Timer_Set_Time(init_sec, init_min, init_hour);
	
	EnableInterrupts();	// Enable interrupts
	Mobile_send_progress(0);
	
	int sw_val;
	int prev_slider_val = 0;
	int slider_val;
	int prev_secs = -1;
	Grahpics_DrawMainMenu();
	Sound_StartUp();
	
	while(true) {
		// Get values from mobile or physical interface
		mobile_mode = Mobile_get_sw2_down();
		sw_val = mobile_mode ? Mobile_get_sw1_down() : Switch_get_sw1_down();
		slider_val = mobile_mode ? Mobile_get_slider() : Switch_get_pot_ADC();
		if (sw_val == 1) {
			// Pour drink
			while (sw_val == 1) {
				// Debounce
				sw_val = mobile_mode ? Mobile_get_sw1_down() : Switch_get_sw1_down();
				DelayWait10ms(10);				
			}
			PourDrink(slider_val);
			Sound_Done();
			Grahpics_DrawMainMenu();
		} else {
			// Update bar if slider has changed
			if (slider_val != prev_slider_val || prev_secs != Timer_Seconds) {
				Graphics_UpdateMainMenu(Timer_Hours, Timer_Minutes, Timer_Seconds, slider_val, 100);
				prev_slider_val = slider_val;
				prev_secs = Timer_Seconds;
			}
		}
	}
}


// ***************************************************************
// **************************** TESTS ****************************
// ***************************************************************


// Simple display test.
void lcd_test() {
	//Graphics_init();
	//Pause();
	//DrawMainMenu();
	//Pause();
	//UpdateMainMenu(1, 2, 3);
	//Pause();
	Graphics_ProgressDisplayInit();
	Pause();
	DelayWait10ms(10);
	for (int i = 0; i <= 100; i ++) {
		Graphics_ProgressDisplayUpdate(i, 100);
		DelayWait10ms(10);
	}
	DelayWait10ms(10);
}

// Tests sound by playing song.
void sound_test() {
	Sound_StartUp();
	Pause();
	Sound_Done();
	Pause();
}

// Tests each pump by turning on and off.
void pump_test() {
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_SetCursor(0, 0);
	ST7735_OutString("Pump 1 Test");
	ST7735_SetCursor(0, 1);
	ST7735_OutString("PF4 to continue");
	Pump1_On();
  Pause();
	Pump1_Off();
	ST7735_SetCursor(0, 0);
	ST7735_OutString("Pump 2 Test");
	ST7735_SetCursor(0, 1);
	ST7735_OutString("PF4 to continue");
	Pump2_On();
	Pause();
	Pump2_Off();
}

// Tests hardware interface by testing their values.
void switch_test() {
	ST7735_FillScreen(ST7735_BLACK);
	int sw1_on = -1;
	int sw2_on = -1;
	int pot_value = -1;
	ST7735_SetCursor(0, 3);
	ST7735_OutString("PF4 to continue");
	while(PF4==0x00) {
		DelayWait10ms(10);
	}
	while(PF4==0x10){
    if (sw1_on != 1 && Switch_get_sw1_down() == 1) {
			sw1_on = 1;
			ST7735_SetCursor(0, 0);
			ST7735_OutString("SW1: on   ");
		}
		if (sw1_on != 0 && Switch_get_sw1_down() == 0) {
			sw1_on = 0;
			ST7735_SetCursor(0, 0);
			ST7735_OutString("SW1: off   ");
		}
		if (sw2_on != 1 && Switch_get_sw2_down() == 1) {
			sw2_on = 1;
			ST7735_SetCursor(0, 1);
			ST7735_OutString("SW2: on   ");
		}
		if (sw2_on != 0 && Switch_get_sw1_down() == 0) {
			sw2_on = 0;
			ST7735_SetCursor(0, 1);
			ST7735_OutString("SW2: off   ");
		}
		if (pot_value != Switch_get_pot_ADC()) {
			pot_value = Switch_get_pot_ADC();
			ST7735_SetCursor(0, 2);
			ST7735_OutString("                ");
			ST7735_SetCursor(0, 2);
			ST7735_OutString("Pot value: ");
			ST7735_OutUDec(pot_value);
		}
		DelayWait10ms(10);
  }
}

// Tests mobile interface by displaying virtual pin values and sending progress values.
void esp_test() {
	ST7735_FillScreen(ST7735_BLACK);
	int vp1 = -1;
	int vp2 = -1;
	int slider = -1;
	int progress = 0;
	int count = 0;
	while(PF4==0x00) {
		DelayWait10ms(10);
	}
	while(PF4==0x10){
		// Display input values every time they change.
		if (vp1 != Mobile_get_sw1_down()) {
			vp1 = Mobile_get_sw1_down();
			ST7735_SetCursor(0, 0);
			ST7735_OutString("VP1 val: ");
			ST7735_OutUDec(vp1);
		}
		if (vp2 != Mobile_get_sw2_down()) {
			vp2 = Mobile_get_sw2_down();
			ST7735_SetCursor(0, 1);
			ST7735_OutString("VP2 val: ");
			ST7735_OutUDec(vp2);
		}
		if (Mobile_get_slider() != slider) {
			slider = Mobile_get_slider();
			ST7735_SetCursor(0, 2);
			ST7735_OutString("Slider val: ");
			ST7735_OutUDec(slider);
		}
		// Test transmission every 100 loops.
		if (count == 100) {
			count = 0;
			progress = (progress + 1) % 100;
			Mobile_send_progress(progress);
		}
		count++;
		DelayWait10ms(10);
	}
}

// Tests timer by displaying current time.
void timer_test(void) {
	int last_sec = -1;
	ST7735_FillScreen(ST7735_BLACK);
	while(PF4==0x00) {
		DelayWait10ms(10);
	}
	while(PF4==0x10){
		if (last_sec != Timer_Seconds) {
			ST7735_SetCursor(0, 0);
			ST7735_OutString("Time: ");
			ST7735_OutUDec(Timer_Hours);
			ST7735_OutString(":");
			ST7735_OutUDec(Timer_Minutes);
			ST7735_OutString(":");
			ST7735_OutUDec(Timer_Seconds);
			last_sec = Timer_Seconds;
		}
		DelayWait10ms(10);
	}
}

void mainMenuTest(void){
	Grahpics_DrawMainMenu();
	Graphics_UpdateMainMenu(16, 12, 0, 30, 40);
	// Graphics_UpdateMainMenu will eventually be in a while loop
	// that sends ADC data (for ratio bar) and timer data (for current time)
	int prop = 0;
	while(prop < 100) {
		Graphics_UpdateMainMenu(1, 2, 3, prop, 100);
		DelayWait10ms(10);
		prop = (prop + 1);
	}
	Pause();
}

int testmain() {
	PLL_Init(Bus80MHz);   // Bus clock at 80 MHz
	DisableInterrupts();  // Disable interrupts until finished with inits
	
	// Low level driver initializations
  LaunchPad_Init();
	Sound_Init();
	Switch_init();	//comment out if using simulator
	Graphics_init();
  Pump_Init();
	Timer_Init();
	
	// Mobile initialization and real-time fetch
	int init_hour, init_min, init_sec;
	Mobile_init(&init_hour, &init_min, &init_sec); //comment out if using simulator
	Timer_Set_Time(init_sec, init_min, init_hour);
	
	EnableInterrupts();	// Enable interrupts
	
	// Driver tests, press PF4 to go from one to the next.
	while(1) {
		sound_test();
		lcd_test();
		mainMenuTest();
		esp_test();
		timer_test();
		switch_test();
		sound_test();
		pump_test();
	}
}
