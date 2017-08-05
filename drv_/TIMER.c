#include "TIMER.h"
volatile unsigned long timer0_overflow_count = 0;
volatile unsigned long timer0_millis = 0;
static unsigned char timer0_fract = 0;
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
unsigned long micros() {
	unsigned long m;
	uint8_t oldSREG = SREG, t;
	
	cli();
	m = timer0_overflow_count;
	t = TCNT0;

	
	if ((TIFR0 & _BV(TOV0)) && (t < 255))
	m++;

	SREG = oldSREG;
	
	return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
}

unsigned long millis()
{
	unsigned long m;
	uint8_t oldSREG = SREG;

	cli();
	m = timer0_millis;
	SREG = oldSREG;

	return m;
}
ISR(TIMER0_OVF_vect)//10 microsecconed timer interrupt
{
	unsigned long m = timer0_millis;
	unsigned char f = timer0_fract;

	m += MILLIS_INC;
	f += FRACT_INC;
	if (f >= FRACT_MAX) {
		f -= FRACT_MAX;
		m += 1;
	}


	timer0_fract = f;
	timer0_millis = m;
	timer0_overflow_count++;
}

ISR(TIMER5_COMPA_vect)//motor update interrupt routine
{
	PWM_update();
}