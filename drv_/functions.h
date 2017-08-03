/*
 * functions.h
 *
 * Created: 7/12/2017 5:18:41 PM
 *  Author: rafa
 */ 
//vahagn comment
#include <avr/io.h>
#include "defines.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <math.h>
void init_gpio();
void getSinTable(uint16_t ,uint8_t *,uint16_t );
double min(double, double, double);
double max(double, double, double);
