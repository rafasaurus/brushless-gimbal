/*
 * UART.cpp
 *
 * Created: 7/16/2017 10:47:13 PM
 *  Author: rafa
 */ 
#include "USART.h"
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "defines.h"
void USART_Init(unsigned int ubrr)
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
	UCSR0B = (1<<TXEN0)| (1<<RXCIE0)|(1<<RXEN0);// | (1<<TXCIE0);
	UCSR0C = (1<<USBS0) | (3<<UCSZ00);//2 stop bit 8 bit
}

unsigned char USART_receive(void){
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}
void print16(int16_t *value)
//this is pointer value, transmited value
//must be reference type &
{
	char c[10];
	itoa(*value, c, 10);
	printf(c);
	//printf("\n");
}

void print16ln(int16_t *value)
//this is pointer value, transmited value
//must be reference type &
{
	char c[10];
	itoa(*value, c, 10);
	printf(c);
	printf("\n");
}

int uart_putchar(char c, FILE *stream)
{
	if (c == '\n')
	uart_putchar('\r', stream);
	while (!( UCSR0A & (1<<UDRE0)));

	UDR0 = c;
	return 0;
}
void printSI(char c[20],int16_t intToPrint)
{
	printf(c);
	print16(&intToPrint);
	printf("  ");
}
void printDouble(double *value)
{
	char c[20]={0};
	sprintf(c, "%f", *value);
	printf(c);
}
void printSD(char c[20],double doubleToPrint)
{
	printf(c);
	printDouble(&doubleToPrint);
	printf("  ");
}
