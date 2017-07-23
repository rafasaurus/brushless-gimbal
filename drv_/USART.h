/*
 * UART.h
 *
 * Created: 7/16/2017 10:47:01 PM
 *  Author: rafa
 */ 
#include <stdio.h>
void USART_Init(unsigned int ubrr);
unsigned int ADC_read();
void USART_Transmit( unsigned char data );
//int USART0SendByte(char u8Data, FILE *stream);