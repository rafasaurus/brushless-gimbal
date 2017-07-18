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
void PWM_update();
int USART0SendByte(char u8Data);
uint8_t currentEnableA=0;
uint8_t currentEnableB=85;
uint8_t currentEnableC=170;
uint8_t currentStepA=0;
uint8_t currentStepB=0;
uint8_t currentStepC=0;

uint8_t lowByte=0;
uint16_t value=0;
uint8_t debug_8bit=0;

uint8_t state_counter=0;
uint8_t phase_state=1;//global state 1,2,3,4,5,6

int USART0SendByte(char u8Data)
{
	if(u8Data == '\n')
	{
		USART0SendByte('\r');
	}
	//wait while previous byte is completed
	while(!(UCSR0A&(1<<UDRE0))){};
	// Transmit data
	UDR0 = u8Data;
	return 0;
}


int main(void)
{	
	ADC_Init();
	USART_Init(MY_UBRR);
//	Enable_timer3_interrupt();
	init_gpio();
	OCR3A=127;
	OCR3B=127;
	OCR3C=127;   
	//GTCCR = 0;//release all timers
	//TCNT1=0;
//	setup_timer3();
	DDRB|=(1<<7);
	DDRG|=(1<<5);
	//UDR0=0;
	sei();
	char Sensor[]="Sensor = ";
	
	
	
    while (1) 
    {
		//for(int i=0x00;i<0xFF;i++)
		//UDR0='A';
		//_delay_ms(1000);
		//while ((UCSR0A & (1 << TXC0)) == 0) ;
		
		//PORTB^=(1<<5);
	    //PORTB^=(1<<6); 
		//UDR0=0x01;  
	//    ADMUX|=(1<<MUX0);
		ADCSRA |= (1<<ADSC);
		//for (int i=0;i<strlen(Sensor);i++)
		//{
		//USART0SendByte(Sensor[i]);	
		//}
		
		_delay_ms(100);
		//// wait until ADC conversion is complete
		//while( ADCSRA & (1<<ADSC) );
		//value=ADCH;
		
		
    }
}

ISR (USART0_TX_vect)//USART interrupt routine
{
		
}

ISR(TIMER3_OVF_vect)//Timer interrupt routine
{
	PWM_update();
}

ISR(ADC_vect)//ADC interrupt routine
{
	value=ADC;
	UDR0=ADC;
}




void PWM_update()
{
	//PWM2->HS_U=11 pin OC2A
	//PWM4->HS_V=6	pin OC0A
	//PWM6->HS_W=5	pin OC0B
	
	//if(++state_counter>5)
	//{
	//	state_counter=0;
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
				//currentStepA++;
				//currentStepB++;
				//currentStepC++;
				phase_state++;
			break;

			case 2:
				//100
				//HS_U=sin_Drv[currentStepA];
				HS_U_HIGH;
				//HS_V_LOW;
				HS_W_LOW;
				LS_U_HIGH;
				LS_V_LOW;
				LS_W_HIGH;
				phase_state++;
				//currentStepA++;
				//currentStepB++;
				//currentStepC++;
			break;

			case 3:
				//101
				//HS_U=sin_Drv[currentStepA];
				HS_U_HIGH;
				HS_V_LOW;
				//HS_W_LOW;
				LS_U_HIGH;
				LS_V_HIGH;
				LS_W_LOW;
				//currentStepA++;
				//currentStepB++;
				//currentStepC++;
				phase_state++;
			break;

			case 4:
				//001
				//HS_U_LOW;
				HS_V_LOW;
				HS_W_HIGH;
				LS_U_LOW;
				LS_V_HIGH;
				LS_W_HIGH;
				phase_state++;
				//currentStepA++;
				//currentStepB++;
				//currentStepC++;
			break;

			case 5:
				//011
				HS_U_LOW;
				//HS_V_LOW;
				HS_W_HIGH;
				LS_U_HIGH;
				LS_V_LOW;
				LS_W_HIGH;
				//currentStepA++;
				//currentStepB++;
				//currentStepC++;
				phase_state++;
			break;

			case 6:
				//010
				HS_U_LOW;
				HS_V_HIGH;
				//HS_W_LOW
				LS_U_HIGH;
				LS_V_HIGH;
				LS_W_LOW;
				phase_state=1;
				//currentStepA++;
				//currentStepB++;
				//currentStepC++;
			break;
		}

	
}



