/*
 * TIMER.cpp
 *
 * Created: 7/18/2017 4:45:52 PM
 *  Author: rafa
 */ 
#include "TIMER.h"


void setup_timer3(void)
{
	//64 pres, 250.000 hz 0.000004s
	sbi (TCCR3B, CS30);
	sbi (TCCR3B, CS31);
	//sbi (TCCR3B, CS32);
	sbi (TCCR3A, WGM30);//8bit
}

void Enable_timer3_interrupt()
{
	sbi (TIMSK3, TOIE3);
}

void setup_timer0(void)
{
	//64 pres, 250.000 hz 0.000004s
	sbi (TCCR0B, CS00);
	sbi (TCCR0B, CS01);
	sbi (TCCR0A, WGM02);//OCR4A compare interrupt
}

void Enable_timer0_compare_interrupt()
{
	sbi (TIMSK0, OCIE0A);
}