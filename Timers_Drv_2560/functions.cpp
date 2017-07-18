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

void setup_timer3(void)
{
	//64 pres, 250.000 hz 0.000004s
	sbi (TCCR3B, CS30);
	sbi (TCCR3B, CS31);	
	//sbi (TCCR3B, CS32);
	
	//sbi (TCCR3A, COM3A1);
	//sbi (TCCR3A, COM3B1);
	//sbi (TCCR3A, COM3C1);
	sbi (TCCR3A, WGM30);//8bit
	//sbi (TCCR3A, WGM31); // Mode 1 / Phase Correct PWM
	//sbi (TCCR3B, WGM33);
	
	//sbi (TCCR3B,WGM32);
	
}


void init_gpio()
{
	/*
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
	*/
	//DDRB|=(1<<PB5);//LED13
	sbi(DDRB,4);//digital 10
	sbi(DDRB,5);//digital 11
	sbi(DDRB,6);//digital 12
	sbi(DDRH,4);//digital 7
	sbi(DDRH,5);//digital 8
	sbi(DDRH,6);//digital 9
}


void Enable_timer3_interrupt()
{
	sbi (TIMSK3, TOIE3);
	//sei();
}





