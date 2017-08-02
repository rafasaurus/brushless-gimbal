#define clockCyclesToMicroseconds(a) ( ((a) * 1000L) / (F_CPU / 1000L) )
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)
// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

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
void setup_timer0();
void Enable_timer0_overflow_interrupt();
unsigned long micros();
unsigned long millis();
//volatile unsigned long timer2_overflow_count=0;
volatile unsigned long timer0_overflow_count = 0;
volatile unsigned long timer0_millis = 0;
static unsigned char timer0_fract = 0;
#define clockCyclesPerMicrosecond() (F_CPU / 1000000L)
FILE * uart_str;
int main(void)
{
	

    //sbi(DDRB,4);//digital 10
	cli();
	USART_Init(MY_UBRR);
	uart_str = fdevopen(uart_putchar, NULL);
	setup_timer0();
	Enable_timer0_overflow_interrupt();
	
	sei();

	//printf("asd \n");
    while (1) 
    {
		unsigned long timer1=micros();
		_delay_us(100);
		uint16_t killer=micros()-timer1;
		print16(&killer);
		printf("\n");
	}
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
void setup_timer0(void)
{
	sbi (TCCR0B, CS00);//only this 8
	sbi (TCCR0B, CS01);
	//sbi (TCCR2B,WGM02);
	//sbi (TCCR2B,WGM22);
	//sbi (TCCR2B, CS22);
	//sbi (TCCR3B, CS31);//only this 256
	//sbi (TCCR0B, WGM02);//OCR4A compare interrupt
}
void Enable_timer0_overflow_interrupt()
{
	sbi (TIMSK0, TOIE0);
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