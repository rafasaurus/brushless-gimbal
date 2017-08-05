#ifndef USART_HEADER 
#define USART_HEADER
#include <stdio.h>
void USART_Init(unsigned int ubrr);
unsigned int ADC_read();
void USART_Transmit( unsigned char data );
//int USART0SendByte(char u8Data, FILE *stream);
FILE * uart_str;
int uart_putchar(char c, FILE *stream);
void printDouble(double *value);
void print16(int16_t *value);
void print16ln(int16_t *value);
void printSI(char c[20],int16_t intToPrint);//print string and integer
#endif