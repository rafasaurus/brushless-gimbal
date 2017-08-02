/*
 * TIMER.cpp
 *
 * Created: 7/18/2017 4:45:52 PM
 *  Author: rafa
 */ 
#include "TIMER.h"

#define clockCyclesPerMicrosecond () ( F_CPU / 1000000L )

/*-----------Timter 1-----------*/
void setup_timer4(void){
    sbi (TCCR4B, CS40);//prescakaer 256
    sbi (TCCR4A, WGM40);//8 bit phase correct PWM
	
	//sbi (TCCR4A,COM4A0);
	sbi (TCCR4A,COM4A1);
	//sbi (TCCR4A,COM4B0);
	sbi (TCCR4A,COM4B1);
	//sbi (TCCR4A,COM4C0);
	sbi (TCCR4A,COM4C1);
	
}


/*-----------Timter 5-----------*/
void setup_timer5(void){
	sbi (TCCR5B, CS50);//only this 8
	//sbi (TCCR5B, CS52);//only this 256
	sbi (TCCR5B, WGM52);//OCR4A compare interrupt
	
}
void Enable_timer5_compare_interrupt(){
	sbi (TIMSK5, OCIE5A);
}
/*-----------Timter 2-----------*/
void setup_timer0(void){
	sbi(TCCR0B,CS00);
	sbi(TCCR0B,CS01);;//prescaler 64 , 250khz timer
}
void Enable_timer0_overflow_interrupt(){
	sbi(TIMSK0,TOIE0);
}
