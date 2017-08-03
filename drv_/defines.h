/*
 * defines.h
 *
 * Created: 7/12/2017 5:16:44 PM
 *  Author: rafa
 */ 

#define  F_CPU 16000000UL
//#define BAUD 74880UL
#define BAUD 57600
#define MY_UBRR (F_CPU/16/BAUD-1)

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define pinChangeHigh(reg, pin) (reg|=(1<<pin))
#define pinChangeLow(reg, pin)  (reg&=~(1<<pin))

#define MPU6050_WRITE (0x68<<1) 
#define MPU6050_READ ((0x68<<1)|1)


#define HS_U OCR2A //pin11
#define HS_V OCR0A //pin6
//#define HS_V OCR2B //pin 3
#define HS_W OCR0B //pin5


#define HS_U_HIGH	sbi(PORTB,4);//pin 10
#define HS_U_LOW	cbi(PORTB,4);
#define HS_V_HIGH	sbi(PORTB,5);//pin 11
#define HS_V_LOW	cbi(PORTB,5);
#define HS_W_HIGH	sbi(PORTB,6);//pin 12
#define HS_W_LOW	cbi(PORTB,6);

#define HS_U_INVERSE (PORTB^=(1<<4))
#define HS_V_INVERSE (PORTB^=(1<<5))
#define HS_W_INVERSE (PORTB^=(1<<6))

#define LS_U_HIGH sbi(PORTH,4)
#define LS_V_HIGH sbi(PORTH,5) 
#define LS_W_HIGH sbi(PORTH,6) 
#define LS_U_LOW cbi(PORTH,4)
#define LS_V_LOW cbi(PORTH,5)
#define LS_W_LOW cbi(PORTH,6)

#define U_PWM OCR4A
#define V_PWM OCR4B
#define W_PWM OCR4C


#define TIMER5_FREQ 250000//hz
#define TIMER5_TO_US(OCR_) ((F_CPU/(2*TIMER5_FREQ))-1)
//_delay_us();
//EN1 = LS_U	digital 7 PORTD ^= (1<<7);
//EN2 = LS_V	digital 8 PORTB ^=(1<<0);
//EN3 = LS_W 	digital 2 PORTD ^=(1<<2);
//#define LS_U_HIGH (PORTD|=(1<<7))
//#define LS_U_LOW (PORTD&=~(1<<7))
//
//#define LS_V_HIGH (PORTB|=(1<<0))
//#define LS_V_LOW (PORTB&=~(1<<0))
//
//#define LS_W_HIGH (PORTD|=(1<<2))
//#define LS_W_LOW (PORTD&=~(1<<2))

#define clockCyclesToMicroseconds(a) ( ((a) * 1000L) / (F_CPU / 1000L) )
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)
// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define pi (3.14159265359)

#define SINE_TABLE_SZ 720
#define sinScale 127//this value is mid range value
#define phase (SINE_TABLE_SZ/3)
#define U_step_predefine 0
#define V_step_predefine (U_step_predefine+phase)
#define W_step_predefine (V_step_predefine+phase)
#define SVPWM