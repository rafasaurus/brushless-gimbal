/*
 * functions.cpp
 *
 * Created: 7/12/2017 5:18:53 PM
 *  Author: rafa
 */ 

#include "functions.h"

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
	//sbi(DDRB,4);//digital 10
	//sbi(DDRB,5);//digital 11
	//sbi(DDRB,6);//digital 12
	sbi(DDRH,3);//digital 6 OC4A
	sbi(DDRH,4);//digital 7 OC4B
	sbi(DDRH,5);//digital 8 OC4C
	//sbi(DDRH,6);//digital 9
}






