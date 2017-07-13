/*
 * defines.h
 *
 * Created: 7/12/2017 5:16:44 PM
 *  Author: rafa
 */ 

#define  F_CPU 16000000UL



#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define SINE_TABLE_SZ 255

#define HS_U OCR2A //pin11
#define HS_V OCR0A //pin6
//#define HS_V OCR2B //pin 3
#define HS_W OCR0B //pin5



//_delay_us();
//EN1 = LS_U	digital 7 PORTD ^= (1<<7);
//EN2 = LS_V	digital 8 PORTB ^=(1<<0);
//EN3 = LS_W 	digital 2 PORTD ^=(1<<2);
#define LS_U_HIGH (PORTD|=(1<<7))
#define LS_U_LOW (PORTD&=~(1<<7))

#define LS_V_HIGH (PORTB|=(1<<0))
#define LS_V_LOW (PORTB&=~(1<<0))

#define LS_W_HIGH (PORTD|=(1<<2))
#define LS_W_LOW (PORTD&=~(1<<2))