/*
 * GccApplication2.c
 *
 * Created: 7/31/2017 1:57:04 PM
 * Author : HP
 */ 
#define F_CPU 16000000
#define BAUD 57600UL
#define MY_UBRR (F_CPU/16/BAUD-1)
#include <avr/io.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
static int uart_putchar(char c, FILE *stream);
unsigned long micros();
void USART_Init(unsigned int ubrr);
void USART_Transmit( unsigned char data );
void print16(uint16_t *value);
volatile unsigned long timer1_micros;
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define HS_U_INVERSE (PORTB^=(1<<4));
unsigned long _micros=0;
void setup_timer2();
void Enable_timer2_overflow_interrupt();
unsigned long micros();
volatile unsigned long timer2_overflow_count=0;
#define clockCyclesPerMicrosecond() (F_CPU / 1000000L)
FILE * uart_str;
int main(void)
{
	

    //sbi(DDRB,4);//digital 10
	cli();
	USART_Init(MY_UBRR);
	uart_str = fdevopen(uart_putchar, NULL);
	setup_timer2();
	Enable_timer2_overflow_interrupt();
	
	sei();

	printf("asd \n");
    while (1) 
    {
	unsigned long timer=micros();
	printf("asd \n");
	//for (int i=0;i<1000;i++);
	//reg=(micros()-timer);
	while(micros()-timer<2000000);
	
    }
}
ISR(TIMER2_OVF_vect)//10 microsecconed timer interrupt
{
	++timer2_overflow_count;
}
void setup_timer2(void)
{
	sbi (TCCR2B, CS20);//only this 8
	sbi (TCCR2B, CS21);
	//sbi (TCCR2B, CS22);
	//sbi (TCCR3B, CS31);//only this 256
	//sbi (TCCR0B, WGM02);//OCR4A compare interrupt
}
void Enable_timer2_overflow_interrupt()
{
	sbi (TIMSK2, TOIE2);
}

unsigned long micros() {
	unsigned long m;
	uint8_t oldSREG = SREG, t;
	
	cli();
	m = timer2_overflow_count;
	t = TCNT0;
	

	#ifdef TIFR0
	if ((TIFR0 & _BV(TOV0)) && (t < 255))
	m++;
	#else
	if ((TIFR0 & _BV(TOV0)) && (t < 255))
	m++;
	#endif

	SREG = oldSREG;
	
	return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
}
		

void USART_Init(unsigned int ubrr)
{
  /* Set baud rate */
  UBRR0H = (unsigned char)(ubrr>>8);
  UBRR0L = (unsigned char)ubrr;
  
  UCSR0B = (1<<TXEN0);// | (1<<TXCIE0);
  UCSR0C = (1<<USBS0) | (3<<UCSZ00);//2 stop bit 8 bit
}


void USART_Transmit( unsigned char data )
{
  /* Wait for empty transmit buffer */
  while ( !( UCSR0A & (1<<UDRE0)) )
  ;
  /* Put data into buffer, sends the data */
  UDR0 = data;
}
void print16(uint16_t *value)
//this is pointer value, transmited value
//must be reference type &
{
	char c[10];
	itoa(*value, c, 10);
	printf(c);
	//printf("\n");
}
static int uart_putchar(char c, FILE *stream)
{

	if (c == '\n')
	uart_putchar('\r', stream);
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = c;
	return 0;
}