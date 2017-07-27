/*
 * TIMER.h
 *
 * Created: 7/18/2017 4:45:37 PM
 *  Author: rafa
 */ 
#include <avr/io.h>
#include "defines.h"

void setup_timer3(void);
void Enable_timer3_interrupt();
void setup_timer0(void);
void Enable_timer0_compare_interrupt();