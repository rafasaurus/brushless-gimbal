#ifndef TIMER_HEADER
#define TIMER_HEADER 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "defines.h"
#include "functions.h"
#define clockCyclesPerMicrosecond() (F_CPU / 1000000L)
unsigned long micros();
unsigned long millis();
void setup_timer3(void);
void setup_timer4(void);
void setup_timer5(void);
void Enable_timer5_compare_interrupt(void);
void setup_timer1(void);
void Enable_timer1_compare_interrupt(void);
void setup_timer0(void);
void Enable_timer0_overflow_interrupt(void);
#endif