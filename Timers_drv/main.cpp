/*
 * Timers_drv.cpp
 *
 * Created: 4/13/2017 3:15:05 PM
 * Author : rafa
 */ 

//#include "Init.h"
#include "defines.h"
#include "functions.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>


void setup_timer2();
void setup_timer1();
void setup_timer0();

void init_gpio();
void Enable_timer2_interrupt();
void Enable_timer0_interrupt();
void PWM_update();

uint8_t currentEnableA=0;
uint8_t currentEnableB=85;
uint8_t currentEnableC=170;
uint8_t currentStepA=0;
uint8_t currentStepB=0;
uint8_t currentStepC=0;
uint8_t sin_Drv[] {
	127,130,133,136,139,143,146,149,152,155,158,161,164,167,170,173,176,178,181,184,187,190,192,195,198,200,203,205,208,210,212,215,217,219,221,223,225,227,229,231,233,234,236,238,239,240,
	242,243,244,245,247,248,249,249,250,251,252,252,253,253,253,254,254,254,254,254,254,254,253,253,253,252,252,251,250,249,249,248,247,245,244,243,242,240,239,238,236,234,233,231,229,227,225,223,
	221,219,217,215,212,210,208,205,203,200,198,195,192,190,187,184,181,178,176,173,170,167,164,161,158,155,152,149,146,143,139,136,133,130,127,124,121,118,115,111,108,105,102,99,96,93,90,87,84,81,78,
	76,73,70,67,64,62,59,56,54,51,49,46,44,42,39,37,35,33,31,29,27,25,23,21,20,18,16,15,14,12,11,10,9,7,6,5,5,4,3,2,2,1,1,1,0,0,0,0,0,0,0,1,1,1,2,2,3,4,5,5,6,7,9,10,11,12,14,15,16,18,20,21,23,25,27,29,31,
	33,35,37,39,42,44,46,49,51,54,56,59,62,64,67,70,73,76,78,81,84,87,90,93,96,99,102,105,108,111,115,118,121,124
};
uint8_t state_counter=0;
uint8_t phase_state=1;//global state 1,2,3,4,5,6
int main(void)
{
	setup_timer0();
	setup_timer1();
	setup_timer2();
	
	Enable_timer2_interrupt();
	//Enable_timer0_interrupt();
	
	//
	init_gpio();
	OCR0A=255;
	OCR0B=255;
	OCR1A=255;
	OCR1B=255;
	OCR2A=255;
	OCR2B=255;
	
    //GTCCR = 0;//release all timers
	TCNT0=0;
	TCNT1=85;
	TCNT2=170;
	
  //_delay_ms(80);
	//#define cycle 
	
	
    while (1) 
    {
	   

    }
}



ISR(TIMER2_OVF_vect)
{
	PWM_update();
}

void PWM_update()
{
	//PWM2->HS_U=11 pin OC2A
	//PWM4->HS_V=6	pin OC0A
	//PWM6->HS_W=5	pin OC0B
	
	if(++state_counter>5)
	{
		state_counter=0;
		switch(phase_state)
		{
			case 1:
				//110
				HS_U=0;
				HS_V=sin_Drv[currentStepB];
				HS_W=0;
				LS_U_LOW;
				LS_V_LOW;
				LS_W_HIGH;
				currentStepA++;
				currentStepB++;
				currentStepC++;
				phase_state++;
			break;

			case 2:
				//100
				HS_U=sin_Drv[currentStepA];
				HS_V=0;
				HS_W=0;
				LS_U_LOW;
				LS_V_LOW;
				LS_W_HIGH;
				phase_state++;
				//currentStepA++;
				//currentStepB++;
				//currentStepC++;
			break;

			case 3:
				//101
				HS_U=sin_Drv[currentStepA];
				HS_V=0;
				HS_W=0;
				LS_U_LOW;
				LS_V_HIGH;
				LS_W_LOW;
				currentStepA++;
				currentStepB++;
				currentStepC++;
				phase_state++;
			break;

			case 4:
				//001
				HS_U=0;
				HS_V=0;
				HS_W=sin_Drv[currentStepC];
				LS_U_LOW;
				LS_V_HIGH;
				LS_W_LOW;
				phase_state++;
				//currentStepA++;
				//currentStepB++;
				//currentStepC++;
			break;

			case 5:
				//011
				HS_U=0;
				HS_V=0;
				HS_W=sin_Drv[currentStepC];
				LS_U_HIGH;
				LS_V_LOW;
				LS_W_LOW;
				currentStepA++;
				currentStepB++;
				currentStepC++;
				phase_state++;
			break;

			case 6:
				//010
				HS_U=0;
				HS_V=sin_Drv[currentStepB];
				HS_W=0;
				LS_U_HIGH;
				LS_V_LOW;
				LS_W_LOW;
				phase_state=1;
				//currentStepA++;
				//currentStepB++;
				//currentStepC++;
			break;
		}

	}
}





