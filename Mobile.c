// Mobile.c
// Juan, Zach, Mithilesh, and Jungjoon
// TA: Bhagawat Vinay
// Date Created: 4/10/2020
// Last Modified: 4/27/2020
// EE 445L Lab 11
// Runs on TM4C123
// Library for mobile interface for cocktail maker.
// Sends information to Blynk virtual pins via ESP8266.

// Inputs from Blynk:
// VP0: Slider from 0 to 50.
// VP1: Virtual switch 1.
// VP2: Virtual switch 2.
// VP3: WebHook data representing time.

// Outputs to Blynk:
// VP70: Gauge representing progress, from 0 to 100.

#include "../inc/ST7735.h"
#include "Timer2.h"
#include "Timer3.h"
#include "UART.h"
#include "esp8266.h"
#include "string.h"
#include "stdlib.h"
#include "../inc/tm4c123gh6pm.h"

// Blynk I/O data:
int sw1_val = 0;
int sw2_val = 0;
int step_val = 0;
int progress_val = 0;
int last_progress = 0;

// These 6 variables contain the most recent Blynk to TM4C123 message
// Blynk to TM4C123 uses VP0 to VP15
char serial_buf[64];
char Pin_Number[2]   = "99";       // Initialize to invalid pin number
char Pin_Integer[8]  = "0000";     //
char Pin_Float[8]    = "0.0000";   //
uint32_t pin_num; 
uint32_t pin_int;

// Delay 10 ms for debouncing and debugging
// Inputs:  none
// Outputs:  none
void delay(uint32_t n){
  uint32_t volatile time;
  while(n){
    time = 727240*2/91;  // 10msec
    while(time){
      time--;
    }
    n--;
  }
}

// Send data to the Blynk App
// It uses Virtual Pin numbers between 70 and 99
// so that the ESP8266 knows to forward the data to the Blynk App
void TM4C_to_Blynk(uint32_t pin,uint32_t value){
  if((pin < 70)||(pin > 99)){
    return; // ignore illegal requests
  }
// your account will be temporarily halted 
  // if you send too much data
  ESP8266_OutUDec(pin);       // Send the Virtual Pin #
  ESP8266_OutChar(',');
  ESP8266_OutUDec(value);      // Send the current value
  ESP8266_OutChar(',');
  ESP8266_OutString("0.0\n");  // Null value not used in this example
}

 
// This routine periodically receives the Blynk Virtual Pin data via ESP.
void Blynk_to_TM4C(void){int j; char data;
// Check to see if a there is data in the RXD buffer
  if(ESP8266_GetMessage(serial_buf)){  // returns false if no message
    // Read the data from the UART5
#ifdef DEBUG1
    j = 0;
    do{
      data = serial_buf[j];
      UART_OutChar(data);        // Debug only
      j++;
    }while(data != '\n');
    UART_OutChar('\r');        
#endif
           
		// Rip the 3 fields out of the CSV data. The sequence of data from the 8266 is:
		// Pin #, Integer Value, Float Value.
    strcpy(Pin_Number, strtok(serial_buf, ","));
    strcpy(Pin_Integer, strtok(NULL, ","));       // Integer value that is determined by the Blynk App
    strcpy(Pin_Float, strtok(NULL, ","));         // Not used
    pin_num = atoi(Pin_Number);     // Need to convert ASCII to integer
    pin_int = atoi(Pin_Integer);  

		// This VP is the set value
    if(pin_num == 0x00)  {  
			step_val = pin_int;
    }
		// This VP corresponds to SW1
		if(pin_num == 0x01)  {  
			sw1_val = pin_int;
    }
		// This VP corresponds to SW2
		if(pin_num == 0x02)  {  
			sw2_val = pin_int;
    }

#ifdef DEBUG1
    UART_OutString(" Pin_Number = ");
    UART_OutString(Pin_Number);
    UART_OutString("   Pin_Integer = ");
    UART_OutString(Pin_Integer);
    UART_OutString("   Pin_Float = ");
    UART_OutString(Pin_Float);
    UART_OutString("\n\r");
#endif
  }  
}

// Periodically update drink progress if progress has been made.
void SendInformation(void){
	if (last_progress != progress_val) {
		last_progress = progress_val;
		TM4C_to_Blynk(70, progress_val);
	}
}

//***GetCurrentTime***
// Requires: ESP and wifi initialization
// Uses virtual pin 74 to use webhook to read current time from an API
// Returns an array of [Hour, Minutes, Seconds] in integers
void GetCurrentTime(int* hour, int* minute, int* second) {
	TIMER2_CTL_R = 0;	//Disable Timer 2 and 3 to avoid interference
	TIMER3_CTL_R = 0;
	char currHour[2], currMin[2], currSec[2];
	int time[3];
	TM4C_to_Blynk(74, 1);	//Write to VP 74
	delay(1000);
	if(ESP8266_GetMessage(serial_buf)){  // returns false if no message
#ifdef DEBUG1
		int j = 0;
		char data;
		do{
			data = serial_buf[j];
			UART_OutChar(data);        // Debug only
			j++;
		}while(data != '\n');
		UART_OutChar('\r');        
#endif
		while(serial_buf[0] < '0' || serial_buf[0] > '9'){
			strcpy(serial_buf, &serial_buf[1]);
		}
		strcpy(currHour, strtok(serial_buf, ",")); //Read hour as char
		strcpy(currMin, strtok(NULL, ","));        //Read min as char
		strcpy(currSec, strtok(NULL, ","));        //Read sec as char 
		time[0] = atoi(currHour);     			 
		time[1] = atoi(currMin);			//Convert to integers
		time[2] = atoi(currSec);
		
#ifdef DEBUG1
		UART_OutString(" Hour = ");
		UART_OutString(currHour);
		UART_OutString("   Minute = ");
		UART_OutString(currMin);
		UART_OutString("   Second = ");
		UART_OutString(currSec);
		UART_OutString("\n\r");
#endif
	
	}
	else{
		time[0] = 0;
		time[1] = 0;
		time[2] = 0;
	}
	TIMER2_CTL_R = 1;	//Enable Timer 2 and 3 to resume periodic communication between Blynk and TM4C
	TIMER3_CTL_R = 1;
	*hour = time[0];
	*minute = time[1];
	*second = time[2];
}


// Initialize WiFi communication with ESP and set up virtual pins for periodic updates.
// Also gets current time at initialization.
// Input:		None
// Output:	None
void Mobile_init(int* hour, int* minute, int* second) {
  UART_Init(5);         // Enable Debug Serial Port
  UART_OutString("\n\rSetting up Blynk");
  ESP8266_Init();       // Enable ESP8266 Serial Port
  ESP8266_Reset();      // Reset the WiFi module
  ESP8266_SetupWiFi();  // Setup communications to Blynk Server  
  Timer2_Init(&Blynk_to_TM4C,800000); 
  Timer3_Init(&SendInformation,10000000);
	GetCurrentTime(hour, minute, second);
}

// Get whether virtual switch 1 is down or not.
// Input:		None
// Output:	1 if sw1 is down, 0 otherwise.
int Mobile_get_sw1_down(void) {
	return sw1_val;
}

// Get whether virtual switch 2 is down or not.
// Input:		None
// Output:	1 if sw1 is down, 0 otherwise.
int Mobile_get_sw2_down(void) {
	return sw2_val;
}

// Get slider value from virtual pin 0.
// Input:		None
// Output:	Integer from 0 to 50 representing slider value.
int Mobile_get_slider(void) {
	return step_val;
}

// Update progress bar on Blynk by updating value (will automatically send periodically).
// Input:		percent- integer from 0 to 100.
// Output:	None
void Mobile_send_progress(int percent) {
	progress_val = percent;
}

