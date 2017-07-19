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
#include "TIMER.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>

#include <string.h>
void MAX_ADC(uint8_t &ADC_value,uint8_t &max);

//uint8_t currentEnableA=0;
//uint8_t currentEnableB=85;
//uint8_t currentEnableC=170;
//uint8_t currentStepA=0;
//uint8_t currentStepB=0;
//uint8_t currentStepC=0;


uint8_t ADC_value=0;
uint8_t phase_state=1;//global state 1,2,3,4,5,6
uint8_t reverse=0;
uint8_t com=0;
uint8_t ADC_set_max=0;
uint8_t ADC_max=0;
int main(void)
{	
	
	OCR1A=400;
//Counter top value. Freq = 16 MHz/prescaler/(OCR0A + 1)
	ADC_Init();
	USART_Init(MY_UBRR);
	setup_timer3();
	setup_timer0();
	Enable_timer3_interrupt();
	Enable_timer0_compare_interrupt();
	init_gpio();
	//GTCCR = 0;//release all timers
	sei();
    while (1) 
    {
	//for (int i=0;i<256;i++)
	//{
	//	OCR0A=i;
	//	_delay_ms(500);
	//}
	
    }
}



ISR(TIMER1_COMPA_vect)
{
	if (com++>1)
	{
		com=0;
	PWM_update(phase_state);
	SWITCH_PHASE_STATE(phase_state);
	
		  //REVERSE(reverse,phase_state);
		  
	}
	
	//UDR0=OCR1A;
	//HS_U_INVERSE;
}

ISR(ADC_vect)//ADC interrupt routine
{
		ADCSRA |= (1<<ADSC);//start ADC conversion 
		ADC_value=ADC;
		if (ADC_set_max<10)
		{
			ADC_set_max++;
			MAX_ADC(ADC_value,ADC_max);
		}
		else
		{
			OCR1A=ADC_max;
			UDR0=ADC_max;
			ADC_max=ADC_value;
			ADC_set_max=0;
		}
		
		//UDR0=ADC_value;
		//OCR1A=ADC_value;
}

ISR(TIMER3_OVF_vect)//Timer interrupt routine
{	//
	//PWM_update(phase_state);
	//SWITCH_PHASE_STATE(phase_state);
	//UDR0=0x15;
	//HS_U_INVERSE;
}
void MAX_ADC(uint8_t &ADC_value,uint8_t &max)
{
	if (ADC_value>max)
	{
		max=ADC_value;
	}
}

