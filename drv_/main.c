/*
 * Timers_drv.cpp
 *
 * Created: 4/13/2017 3:15:05 PM
 * Author : rafa
 */ 


//#include "Init.h"
#define PRINT_RAW_DATA

#include "defines.h"
#include "functions.h"
#include "USART.h"
#include "ADC.h"
#include "TIMER.h"
#include "mpu6050registers.h"
#include "MPU6050.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

FILE * uart_str;
typedef int bool;
enum { false, true };
bool __ftoa(double val, char * buf, int nLen,uint8_t after_decimal_point);//convert double to char
static int uart_putchar(char c, FILE *stream);
/*----------MPU6050 defines---------*/

/*----------end MPU6050 defines---------*/


//---------------------------------------------

uint16_t ADC_value=0;
uint8_t phase_state=1;//global state 1,2,3,4,5,6
uint8_t reverse=0;
uint8_t com=0;
uint8_t ADC_set_max=0;
uint16_t ADC_max=0;
uint8_t buffer[14];

int main(void)

{	

	USART_Init(MY_UBRR);
	//uart_init(UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));
	uart_str = fdevopen(uart_putchar, NULL);
	//	OCR1A=50;
	//Counter top value. Freq = 16 MHz/prescaler/(OCR0A + 1)
	//ADC_Init();
	//setup_timer0();
	//Enable_timer0_compare_interrupt();
	//init_gpio();
	sei();
	//sbi(ADCSRA,ADSC);
	
	/*----------MPU6050 twi init---------*/
	i2c_init();
	uint16_t gyro_x;
	uint16_t gyro_y;
	uint16_t gyro_z;
	uint16_t accel_x;
	uint16_t accel_y;
	uint16_t accel_z;
	double angle_pitch=0;
	double angle_roll=0;
	printf("\n");
	mpu6050_writeByte(MPU6050_RA_GYRO_CONFIG,0x08);//gyro sensitivity set to 500 o/s
	mpu6050_writeByte(MPU6050_RA_ACCEL_CONFIG,0x10);//accel sensitivity -/+ 8g
	#ifdef CALIBERATED_DATA
		int32_t gyroX_calib=0;
		int32_t gyroY_calib=0;
		int32_t gyroZ_calib=0;
		int32_t accelX_calib=0;
		int32_t accelY_calib=0;
		int32_t accelZ_calib=0;
		mpu6050_calibrate_gyro(&gyroX_calib,&gyroY_calib,&gyroZ_calib);
		mpu6050_calibrate_accel(&accelX_calib,&accelY_calib,&accelZ_calib);
	#endif
	
	uint32_t timer=0;
	/*-----------------end---------------*/
    while (1) //hesa kgam
    {
		time_t timer=time(NULL);
    	mpu6050_getRawData(&accel_x,&accel_y,&accel_z,&gyro_x,&gyro_y,&gyro_z);
			#ifdef CALIBERATED_DATA
				accX;
				accY;
				accZ;
				grX;
				grY;
				grZ;
			#endif
		#ifdef PRINT_RAW_DATA
			/*--------raw data gyro-accel------*/
			
			printf("x= ");
			print16(&gyro_x);
			printf("  ");
			
			printf("y= ");
			print16(&gyro_y);
			printf("  ");
			
			printf("Z= ");
			print16(&gyro_z);
			printf("  ");
			
			
			printf("accX= ");
			print16(&accel_x);
			printf("  ");
			
			printf("accY= ");
			print16(&accel_y);
			printf("  ");
			
			printf("accZ= ");
			print16(&accel_z);
			printf("  ");
			printf("\n");
			/*--------end------*/
			
		#else
			/*
			//Gyro angle calculations
			//0.0000611 = 1 / (250Hz / 65.5)
			angle_pitch += gyro_x * 0.0000611; //Calculate the traveled pitch angle and add this to the angle_pitch variable
			angle_roll += gyro_y * 0.0000611;  //Calculate the traveled roll angle and add this to the angle_roll variable
			
			  //0.000001066 = 0.0000611 * (3.142(PI) / 180degr) The Arduino sin function is in radians
			angle_pitch += angle_roll * sin(gyro_z * 0.000001066);               //If the IMU has yawed transfer the roll angle to the pitch angel
			angle_roll -= angle_pitch * sin(gyro_z * 0.000001066);    */           //If the IMU has yawed transfer the pitch angle to the roll angel
			
			uint16_t var=time(NULL) - timer ;                                 //Wait until the loop_timer reaches 4000us (250Hz) before starting the next loop
			print16(var);
			
			//print_double(&angle_roll);
			printf("\n");
			_delay_ms(10);	
		#endif  
		
	}
	return 0;
}


//ISR(TIMER1_COMPA_vect)
//{
//
//	PWM_update(&phase_state);
//	SWITCH_PHASE_STATE(&phase_state);
//
//}
//
//
//ISR(ADC_vect)//ADC interrupt routine
//{
//		ADC_value=ADC;
//		//print16(ADC_value);
//		OCR1A=ADC_value;
//		sbi(ADCSRA,ADSC);
//}
//void print16(uint16_t *value)
//{
//	uint8_t first_=(uint8_t)(*value/1000);
//	uint8_t seccond_=(uint8_t)((*value-first_*1000)/100);
//	uint8_t third_=(uint8_t)((*value-first_*1000-seccond_*100)/10);
//	uint8_t fourth_=*value-first_*1000-seccond_*100-third_*10;
//	char c;
//	itoa(first_, &c, 10);
//	printf(&c);
//	itoa(seccond_, &c, 10);
//	printf(&c);
//	itoa(third_, &c, 10);
//	printf(&c);
//	itoa(fourth_, &c, 10);
//	printf(&c);
//	printf("\n");
//}

//ISR(TIMER3_OVF_vect)//Timer interrupt routine
//{	//
//	//PWM_update(phase_state);
//	//SWITCH_PHASE_STATE(phase_state);
//	//UDR0=0x15;
//	//HS_U_INVERSE;
//}

void print16(uint16_t *value)
//this is pointer value, transmited value
//must be reference type &
{
	char c[10];
	itoa(*value, c, 10);
	printf(c);
	//printf("\n");
}

void print16ln(uint16_t *value)
//this is pointer value, transmited value
//must be reference type &
{
	char c[10];
	itoa(*value, c, 10);
	printf(c);
	printf("\n");
}
void print_double(double *value)
//this is pointer value, transmited value
//must be reference type &
{
	char c[10];
	__ftoa(*value, c, 10,2);
	printf(c);
}
bool __ftoa(double val, char * buf, int nLen,uint8_t after_decimal_point){
//----------https://rsdn.org/forum/cpp/1539621.all-----------//
	int32_t nValue = val * pow(10,after_decimal_point);

	int32_t nCharCount = after_decimal_point+1; // decimals and point by default is present
	int32_t nCharCounter = nValue;
	while ( nCharCounter > 100 ) {
		nCharCount++;
		nCharCounter /= 10;
	}
	
	if ( nLen < nCharCount )
	return false;

	for ( int32_t i = 0; i < nCharCount; i++ ) {
		if ( after_decimal_point == i ) {
			buf[nCharCount - i - 1] = '.';
			} else 
			{
			buf[nCharCount-i-1]=(int32_t)(nValue%10)+48;
			nValue /= 10;
		}
	}
	return true;
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