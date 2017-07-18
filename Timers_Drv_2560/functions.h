/*
 * functions.h
 *
 * Created: 7/12/2017 5:18:41 PM
 *  Author: rafa
 */ 
//vahagn comment
#include <avr/io.h>
void setup_timer3();
void init_gpio();
void Enable_timer3_interrupt();
void REVERSE(uint8_t &,uint8_t &);
void SWITCH_PHASE_STATE(uint8_t &);