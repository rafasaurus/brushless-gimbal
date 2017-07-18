/*
 * UART.cpp
 *
 * Created: 7/16/2017 10:47:13 PM
 *  Author: rafa
 */ 
#include "USART.h"
#include <avr/io.h>
#include <stdio.h>
void USART_Init(unsigned int ubrr)
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
	UCSR0B = (1<<TXEN0);// | (1<<TXCIE0);
	UCSR0C = (1<<USBS0) | (3<<UCSZ00);//2 stop bit 8 bit
}

