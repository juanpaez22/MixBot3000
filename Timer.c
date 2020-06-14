// Timer.c
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

#include "timer.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/LaunchPad.h"
#include "../inc/CortexM.h"

#define PF2 (*((volatile uint32_t *)0x40025010))	// Heartbeat

//Global Variables
volatile uint8_t Timer_Seconds, Timer_Minutes, Timer_Hours;


// Initializes timer0 to 1 hz and enables its interrupts
// Inputs: None
// Outputs: None
void Timer_Init(void){
	Timer_Seconds = 0;
  Timer_Minutes = 0;
  Timer_Hours = 0;
  volatile uint32_t delay;
  DisableInterrupts();
  // **** general initialization ****
  SYSCTL_RCGCTIMER_R |= 0x01;      // activate timer0
  delay = SYSCTL_RCGCTIMER_R;      // allow time to finish activating
  TIMER0_CTL_R &= ~TIMER_CTL_TAEN; // disable timer0A during setup
  TIMER0_CFG_R = 0;                // configure for 32-bit timer mode
  // **** timer0A initialization ****
                                   // configure for periodic mode
  TIMER0_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
  TIMER0_TAILR_R = 80000000;         // start value for 60 Hz interrupts
  TIMER0_IMR_R |= TIMER_IMR_TATOIM;// enable timeout (rollover) interrupt
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// clear timer0A timeout flag
  TIMER0_CTL_R |= TIMER_CTL_TAEN;  // enable timer0A 32-b, periodic, interrupts
  // **** interrupt initialization ****
                                   // Timer0A=priority 1
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x20000000; // top 3 bits
  NVIC_EN0_R |= 1<<19;              // enable interrupt 19 in NVIC
}

// The ISR for Timer 0 will increase the current time by one second.
void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;    // acknowledge timer0A timeout
  PF2 ^= 0x04;
  Timer_Seconds += 1;
  if(Timer_Seconds >= 60){
    Timer_Seconds = 0;
    Timer_Minutes += 1;
    if(Timer_Minutes >= 60){
      Timer_Minutes = 0;
      Timer_Hours += 1;
      if(Timer_Hours >= 24){
        Timer_Hours = 0;
      }
    }
  }
}

uint8_t Timer_Get_Hours(void){
	return Timer_Hours;
}

uint8_t Timer_Get_Minutes(void){
	return Timer_Minutes;
}

uint8_t Timer_Get_Seconds(void){
	return Timer_Seconds;
}

// Sets global time.
// Input: uint8_t for seconds, minutes, and hours
// Output: 1 if valid input, 0 otherwise
uint8_t Timer_Set_Time(uint8_t sec, uint8_t min, uint8_t hour){
  if (sec > 59 || min > 59 || hour > 23)
    return 0;
  else{
    TIMER0_CTL_R = 0; //Disable timer
    Timer_Seconds = sec;
    Timer_Minutes = min;
    Timer_Hours = hour;
    TIMER0_CTL_R = 0x00000001; //Enable Timer 2A
  }
  return 1;
}
