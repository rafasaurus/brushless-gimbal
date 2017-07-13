/*
 * functions.cpp
 *
 * Created: 7/12/2017 5:18:53 PM
 *  Author: rafa
 */ 
#include "defines.h"
#include "functions.h"


#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>


void setup_timer2() //prescaler 1024    15625hz
{
	
	sbi (TCCR2B, CS20);
	//sbi (TCCR2B, CS21);
	//sbi (TCCR2B, CS22);
	
	sbi (TCCR2A, COM2A1);
	sbi (TCCR2A, COM2B1);
	sbi (TCCR2A, WGM20); // Mode 1 / Phase Correct PWM
	//sbi (TIMSK2, TOIE2);
	//sbi (TIMSK2, TOIE2);
	//sei();
	
}

void setup_timer1(void)
{
	
	sbi (TCCR1B, CS10);
	//sbi (TCCR1B, CS11);
	//sbi (TCCR1B, CS12);
	
	sbi (TCCR1A, COM1A1);
	sbi (TCCR1A, COM1B1);
	sbi (TCCR1A, WGM10); // Mode 1 / Phase Correct PWM
}
void setup_timer0(void)
{
	// Timer1 Clock Prescaler to : 1
	
	sbi (TCCR0B, CS00);
    //sbi (TCCR0B, CS01);
	//sbi (TCCR0B, CS02);
	
	sbi (TCCR0A, COM0A1);
	sbi (TCCR0A, COM0B1);
	sbi (TCCR0A, WGM00); // Mode 1 / Phase Correct PWM
}

void init_gpio()
{
	DDRB|=(1<<PB1);//digital 9 timer1   OC1A 
	DDRB|=(1<<PB2);//digital 10 timer1  OC1B
	DDRD|=(1<<PD3);//digital 3 timer2   OC2B
	DDRB|=(1<<PB3);//digital 11 timer2  OC2A
	DDRD|=(1<<PD5);//digital 6 timer0   OC0A
	DDRD|=(1<<PD6);//digital 5 timer0   OC0B
	//for debugging not pwm 7,8,2 pins used
	DDRD|=(1<<PD7);//enable 1 DIGITAL 7
	DDRB|=(1<<PB0);//enable 2 DIGITAL 8
	DDRD|=(1<<PD2);//enable 3 DIGITAL 2
	
	DDRB|=(1<<PB5);//LED13
}

void Enable_timer2_interrupt()
{
	sbi (TIMSK2, TOIE2);
	sei();
}

void Enable_timer0_interrupt()
{
	sbi (TIMSK0, TOIE0);
	sei();
}



/*
ISR(TIMER2_OVF_vect)
{
	//PORTD &= (1<<PORTD7);
	currentEnableA++;
	currentEnableB++;
	currentEnableC++;
	if (currentEnableA>=255)
	{
		PORTD ^= (1<<7); //digital 7
		currentEnableA=0;//enable 1
	}
	if (currentEnableB>=255)
	{
		PORTB ^=(1<<0);//enable 2 digital 8
		currentEnableB=0;
	}
    if(currentEnableC>=255)
	{
		PORTD ^=(1<<2);//enable 3 digital 2
		currentEnableC=0;
	}
	
}
*/









