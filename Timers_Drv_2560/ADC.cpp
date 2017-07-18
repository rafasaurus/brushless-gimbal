/*
 * ADC.cpp
 *
 * Created: 7/16/2017 10:33:23 PM
 *  Author: rafa
 */ 
#include "ADC.h"
#include "defines.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

void ADC_Init()
{
	//enable interrupts
	//10 bit resolution
	//enable a prescaler , which is 128 , so ADC clock is 125khz
	//turn on the adc feature
	
	//start the first conversion
	//enable the global interrupts
	
	//ADMUX  =(1<<ADLAR);//adlar is direction bit for ADCL , ADCH distribution 
	ADMUX |= (1<<REFS0);
	ADCSRA = (1<<ADEN) | (1<<ADSC) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);//control and status register
	//ADSC is ADC start register maybe after sei() ????
	ADCSRB = (1<<ADTS0) | (1<<ADTS1);//Timer/Counter0 Compare Match A
	DIDR0 = 0xFE;
	DIDR2 = 0xFF;
}

unsigned int ADC_read()
{
	while(ADCSRA & (1<<ADSC));
	return ADC;
}


uint16_t Read_ADC(uint8_t ADCchannel)
{
	//select ADC channel with safety mask
	ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F);
	//single conversion mode
	ADCSRA |= (1<<ADSC);
	// wait until ADC conversion is complete
	while( ADCSRA & (1<<ADSC) );
	return ADC;
}
//void ADC_select(uint8_t chan)
//{
//	ADMUX |= chan & 0x1F;	//select channel (MUX0-4 bits)
//	ADCSRB |= chan & 0x20;	//select channel (MUX5 bit)
//	ADCSRA |= (1<<ADSC);	// ADC Start Conversion
//}