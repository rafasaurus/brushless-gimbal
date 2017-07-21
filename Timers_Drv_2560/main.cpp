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
#include "i2c_master.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>


#include <string.h>
void print16(uint16_t &value);
void init_mpu6050();
FILE * uart_str;
static int uart_putchar(char c, FILE *stream);
static int uart_putchar(char c, FILE *stream)
{

	if (c == '\n')
	uart_putchar('\r', stream);
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = c;
	return 0;
}
//---------------------------------------------

uint16_t ADC_value=0;
uint8_t phase_state=1;//global state 1,2,3,4,5,6
uint8_t reverse=0;
uint8_t com=0;
uint8_t ADC_set_max=0;
uint16_t ADC_max=0;
uint8_t buffer[14];
int main(void)
{	
	uart_str = fdevopen(uart_putchar, NULL);
	OCR1A=50;
	//Counter top value. Freq = 16 MHz/prescaler/(OCR0A + 1)
	ADC_Init();
	i2c_init();
	
	USART_Init(MY_UBRR);
	//setup_timer3();
	setup_timer0();
	//Enable_timer3_interrupt();
	Enable_timer0_compare_interrupt();
	init_gpio();
	//GTCCR = 0;//release all timers
	sei();
	sbi(ADCSRA,ADSC);
    while (1) 
    {
      //init_mpu6050();
	  //printf("d");
	  while(i2c_receive(0x75,buffer,1));
	  if (buffer[0]!=0x68)
	  {
		  //read who am i
		  printf("error");
	  }
	  else
	  {
		  char c;
		  itoa(buffer[0],&c,10);
		  printf(&c);
	  }
    }
}
void init_mpu6050()
{
	buffer[0] = 7; // Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz
	buffer[1] = 0x00; // Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro filtering, 8 KHz sampling
	buffer[2] = 0x00; // Set Gyro Full Scale Range to ±250deg/s
	buffer[3] = 0x00; // Set Accelerometer Full Scale Range to ±2g
	while(i2c_transmit(0x19,buffer,4));
	//while(i2c_transmit((0x6B,0x01,1)));
	while(i2c_receive(0x75,buffer,1));
	if (buffer[0]!=0x68)
	{
		//read who am i
		printf("error");	
	}
	else
	{
		char c;
		itoa(buffer[0],&c,10);
		printf(&c);
	}
	
}


ISR(TIMER1_COMPA_vect)
{

	PWM_update(phase_state);
	SWITCH_PHASE_STATE(phase_state);

}


ISR(ADC_vect)//ADC interrupt routine
{
		ADC_value=ADC;
		//print16(ADC_value);
		OCR1A=ADC_value;
		sbi(ADCSRA,ADSC);
}
void print16(uint16_t &value)
{
	uint8_t first_=(uint8_t)(value/1000);
	uint8_t seccond_=(uint8_t)((value-first_*1000)/100);
	uint8_t third_=(uint8_t)((value-first_*1000-seccond_*100)/10);
	uint8_t fourth_=value-first_*1000-seccond_*100-third_*10;
	char c;
	itoa(first_, &c, 10);
	printf(&c);
	itoa(seccond_, &c, 10);
	printf(&c);
	itoa(third_, &c, 10);
	printf(&c);
	itoa(fourth_, &c, 10);
	printf(&c);
	printf("\n");
}

//ISR(TIMER3_OVF_vect)//Timer interrupt routine
//{	//
//	//PWM_update(phase_state);
//	//SWITCH_PHASE_STATE(phase_state);
//	//UDR0=0x15;
//	//HS_U_INVERSE;
//}

