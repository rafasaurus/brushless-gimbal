#ifndef FUNCTION
#define FUNCTION
#include <avr/io.h>
#include "defines.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <math.h>
void init_gpio();
void init_motor_gpio();
void getSinTable(uint16_t ,uint8_t *,uint16_t );
double min(double, double, double);
double max(double, double, double);
void PWM_update(void);
bool direction;
int incr;//increment variable
extern int U_step;
extern int V_step;
extern int W_step;
#ifdef GENERATE_SIN
uint8_t pwmSin[SINE_TABLE_SZ];
#elif SINPRESCALER
extern uint8_t pwmSin[];
#endif
#endif