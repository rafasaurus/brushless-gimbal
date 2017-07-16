/*
 * UART.cpp
 *
 * Created: 7/16/2017 10:47:13 PM
 *  Author: rafa
 */ 
#include "USART.h"
#include <avr/io.h>
void USART_Init(unsigned int ubrr)
{
	/* Set baud rate */
	UBRR1H = (unsigned char)(ubrr>>8);
	UBRR1L = (unsigned char)ubrr;
	
	UCSR1B = (1<<TXEN1) | (1<<TXCIE1);
	UCSR1C = (1<<USBS1) | (3<<UCSZ10);//2 stop bit 8 bit
}