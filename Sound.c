// Sound.c
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
#include "Speaker.h"
#include "Sound.h"
#include "../inc/tm4c123gh6pm.h"

#define C5 152890
#define D5 136210
#define E5 121349
#define F5s 108110
#define G5 102042
#define G5s  96315
#define A5  90909
#define B5  80909
#define C6  76445
#define D6  68105
#define E6  60675 
#define O 0xFFFFFFFF

volatile uint8_t speakerData;
const uint32_t Period = 13333333;
volatile uint8_t Index;
	
const uint32_t startUp[] = {A5, B5, C6, D6, E6};
volatile uint8_t startUpIndex;
#define startUpLen sizeof(startUp)/sizeof(uint32_t)

#define A6 22727
const uint32_t done[] = {A6, A6, O, O, A6, A6, O, O, A6, A6};
volatile uint8_t doneIndex;
#define doneLen sizeof(done)/sizeof(uint32_t)



// Initialize sound interface and interrupts.
// Inputs:  None
// Outputs:  None
void Sound_Init(void){
	doneIndex = doneLen;
	startUpIndex = startUpLen*4;
	
  speakerData = 0;
  Index = 0;
  Speaker_Init();
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_ST_RELOAD_R = 0x00FFFFFF;
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000; // priority 2
  //NVIC_ST_CTRL_R = 0x0007; // enable SysTick with core clock and interrupts

  //Timer 1A init
  SYSCTL_RCGCTIMER_R |= 0x0002; // activate timer1
  TIMER1_CTL_R &= ~0x00000001;   // disable timer1A during setup
  TIMER1_CFG_R = 0x00000000;     // configure for 32-bit timer mode
  TIMER1_TAMR_R = 0x00000002;   // configure for periodic mode
  TIMER1_TAILR_R = Period;    // 4) reload value (for 8 hz)
  TIMER1_TAPR_R = 0;            // 5) bus clock resolution
  TIMER1_ICR_R = 0x00000001;    // 6) clear timer1A timeout flag
  TIMER1_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R =(NVIC_PRI5_R&0xFFFF00FF)|0x00004000; // priority 2 
  // interrupts enabled in the main program after all devices initialized
  // vector number 37, interrupt number 35
  NVIC_EN0_R |= 0x00200000;           // 9) enable IRQ 21 in NVIC
  
}


// Stop sound play by disabling interrupts.
// Requires SysTick and Timer1A
// Inputs:  None
// Outputs:  None
void Sound_Stop(void){
  TIMER1_CTL_R &= ~0x00000001; //disable Timer2A
  NVIC_ST_CTRL_R &= 0;  //disable SysTick
  Index = 0;
  Speaker_Out(0);
}

static void changeFreq(uint32_t freq){
	NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = freq-1;// reload value
  NVIC_ST_CTRL_R = 0x07;    // enable SysTick
}

// Play short sound on startup
// Input: None
// Output: None
void Sound_StartUp(){
	startUpIndex = 0;
	changeFreq(startUp[startUpIndex]);
  TIMER1_CTL_R |= 0x00000001;    //enable timer2A
}

// Play short sound on drink completion
// Input: None
// Output: None
void Sound_Done(){
	doneIndex = 0;
	changeFreq(done[doneIndex]);
  TIMER1_CTL_R |= 0x00000001;    //enable timer2A
}

// Increments the index for the next note to be played in the song
// Inputs:  None
// Outputs:  None
void Timer1A_Handler(void){
  TIMER1_ICR_R = TIMER_ICR_TATOCINT; //acknoledge
	if(startUpIndex < startUpLen*4 && doneIndex < doneLen){
		startUpIndex = startUpLen * 4;
	}
	
	if(doneIndex < doneLen){
		doneIndex++;
		if(doneIndex == doneLen){
			TIMER1_CTL_R &= ~0x00000001; //disable Timer2A
			NVIC_ST_CTRL_R &= 0;  //disable SysTick
			Speaker_Out(0);
		}
		else{
			changeFreq(done[doneIndex]);
		}
	}
	
	if(startUpIndex < startUpLen*4){
		startUpIndex++;
		if(startUpIndex == startUpLen*4){
			TIMER1_CTL_R &= ~0x00000001; //disable Timer2A
			NVIC_ST_CTRL_R &= 0;  //disable SysTick
			Speaker_Out(0);
		}
		else{
			changeFreq(startUp[startUpIndex/4]);
		}
	}
}
	
//  Index++;
//  if(Index == SongLength)
//    Index = 0;
//  Sound_Play();
//}

// XORs the current output to the speaker
// Inputs:  None
// Outputs:  None
void SysTick_Handler(void){
  speakerData ^= 0x01;       
  Speaker_Out(speakerData);    // output one value each interrupt
}
