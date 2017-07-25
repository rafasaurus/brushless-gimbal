/*
 * TIMER.cpp
 *
 * Created: 7/18/2017 4:45:52 PM
 *  Author: rafa
 */ 
#include "TIMER.h"

#define clockCyclesPerMicrosecond () ( F_CPU / 1000000L )
/*
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
}*/
/*-----------Timter 1-----------*/
void setup_timer1(void)
{
	//64 pres, 250.000 hz 0.000004s
	
	//sbi (TCCR1B, CS10);
	//sbi (TCCR1B, CS11);
    sbi (TCCR1B, CS12);
    sbi (TCCR1B, WGM12);//OCR4A compare interrupt
}

void Enable_timer1_compare_interrupt()
{
	sbi (TIMSK1, OCIE1A);	
}
/*-----------Timter 0-----------*/
void setup_timer3(void)
{
	//64 pres, 250.000 hz 0.000004s
	//sbi (TCCR5B, CS50);//only this no prescaling
	
	sbi (TCCR3B, CS30);//only this 8
	//sbi (TCCR5B, CS52);//only this 256
	sbi (TCCR3B, WGM32);//OCR4A compare interrupt
}
void Enable_timer3_compare_interrupt()
{
	sbi (TIMSK3, OCIE3A);
}
