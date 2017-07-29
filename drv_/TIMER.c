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
void setup_timer4(void)
{
    sbi (TCCR4B, CS40);//prescakaer 256
    sbi (TCCR4A, WGM40);//8 bit phase correct PWM
	
	//sbi (TCCR4A,COM4A0);
	sbi (TCCR4A,COM4A1);
	//sbi (TCCR4A,COM4B0);
	sbi (TCCR4A,COM4B1);
	//sbi (TCCR4A,COM4C0);
	sbi (TCCR4A,COM4C1);
	
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
void setup_timer1(void)
{
	sbi (TCCR5B, CS50);//only this 8
	//sbi (TCCR5B, CS52);//only this 256
	sbi (TCCR5B, WGM52);//OCR4A compare interrupt
	
}
void Enable_timer1_compare_interrupt()
{
	sbi (TIMSK5, OCIE5A);
}