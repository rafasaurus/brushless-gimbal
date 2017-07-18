/*
 * Timers_drv.cpp
 *
 * Created: 4/13/2017 3:15:05 PM
 * Author : rafa
 */ 

//#include "Init.h"
#include "defines.h"
#include "functions.h"
#include "USART.h"
#include "ADC.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>

#include <string.h>

int timer=0;
void PWM_update(uint8_t &);
int USART0SendByte(char u8Data);
uint8_t currentEnableA=0;
uint8_t currentEnableB=85;
uint8_t currentEnableC=170;
uint8_t currentStepA=0;
uint8_t currentStepB=0;
uint8_t currentStepC=0;

uint8_t lowByte=0;
uint16_t ADC_value=0;
uint8_t debug_8bit=0;

uint8_t state_counter=0;

uint8_t phase_state=1;//global state 1,2,3,4,5,6
uint8_t reverse=0;
int main(void)
{	
	
	
	ADC_Init();
	USART_Init(MY_UBRR);
	setup_timer3();
	Enable_timer3_interrupt();
	init_gpio();
	//GTCCR = 0;//release all timers
	sei();
    while (1) 
    {
	
    }
}

ISR(TIMER3_OVF_vect)//Timer interrupt routine
{
	PWM_update(phase_state);
	REVERSE(reverse,phase_state);
}

ISR(ADC_vect)//ADC interrupt routine
{
		ADCSRA |= (1<<ADSC);//start ADC conversion 
		UDR0=ADC;
		//_delay_ms(100);
}

void PWM_update(uint8_t &phase_state)
{
		switch(phase_state)
		{
			case 1:
				//110
				//HS_U_LOW;
				HS_V_HIGH;
				HS_W_LOW;
				
				LS_U_LOW;
				LS_V_HIGH;
				LS_W_HIGH;

			break;

			case 2:
				//100
				HS_U_HIGH;
				//HS_V_LOW;
				HS_W_LOW;
				LS_U_HIGH;
				LS_V_LOW;
				LS_W_HIGH;

			break;

			case 3:
				//101
				HS_U_HIGH;
				HS_V_LOW;
				//HS_W_LOW;
				LS_U_HIGH;
				LS_V_HIGH;
				LS_W_LOW;

			break;

			case 4:
				//001
				//HS_U_LOW;
				HS_V_LOW;
				HS_W_HIGH;
				LS_U_LOW;
				LS_V_HIGH;
				LS_W_HIGH;

			break;

			case 5:
				//011
				HS_U_LOW;
				//HS_V_LOW;
				HS_W_HIGH;
				LS_U_HIGH;
				LS_V_LOW;
				LS_W_HIGH;

			break;

			case 6:
				//010
				HS_U_LOW;
				HS_V_HIGH;
				//HS_W_LOW
				LS_U_HIGH;
				LS_V_HIGH;
				LS_W_LOW;

			break;
		}	
}



