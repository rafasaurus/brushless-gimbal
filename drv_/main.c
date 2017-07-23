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

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

uint16_t mpu6050_read_gyroX();
uint16_t mpu6050_read_gyroY();
uint16_t mpu6050_read_gyroZ();
uint16_t mpu6050_read_accelX();
uint16_t mpu6050_read_accelY();
uint16_t mpu6050_read_accelZ();

void mpu6050_calibrate_gyro(int32_t *x,int32_t *y, int32_t *z);
void mpu6050_calibrate_accel(int32_t *x,int32_t *y, int32_t *z);
uint8_t mpu6050_rb(uint8_t byteToRead);//readbyte
#include <string.h>

#define accX (mpu6050_read_accelX()-accelX_calib)
#define accY (mpu6050_read_accelY()-accelY_calib)
#define accZ (mpu6050_read_accelZ()-accelZ_calib) 
#define grX (mpu6050_read_gyroX()-gyroX_calib)
#define grY (mpu6050_read_gyroY()-gyroY_calib)
#define grZ (mpu6050_read_gyroZ()-gyroZ_calib)
#define calibration_counter 2000
#define caliberation_wait_delay 0//in ms

FILE * uart_str;
static int uart_putchar(char c, FILE *stream);
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
	int32_t gyroX_calib=0;
	int32_t gyroY_calib=0;
	int32_t gyroZ_calib=0;
	int32_t accelX_calib=0;
	int32_t accelY_calib=0;
	int32_t accelZ_calib=0;
	uint16_t gyro_x ;
	uint16_t gyro_y ;
	uint16_t gyro_z ;
	uint16_t accel_x;
	uint16_t accel_y;
	uint16_t accel_z;
	double angle_pitch=0;
	double angle_roll=0;
	printf("\n");
	mpu6050_wb(MPU6050_RA_GYRO_CONFIG,0x08);//gyro sensitivity set to 500 o/s
	mpu6050_wb(MPU6050_RA_ACCEL_CONFIG,0x10);//accel sensitivity -/+ 8g
	mpu6050_calibrate_gyro(&gyroX_calib,&gyroY_calib,&gyroZ_calib);
	mpu6050_calibrate_accel(&accelX_calib,&accelY_calib,&accelZ_calib);
	/*-----------------end---------------*/
    while (1) //hesa kgam
    {
		#ifdef PRINT_RAW_DATA
		/*--------raw data gyro-accel------*/
		uint16_t var=grX;
		gyro_x=var;
		printf("x= ");
		print16(&var);
		printf("  ");
		var=grY;
		gyro_y=var;
		printf("y= ");
		print16(&var);
		printf("  ");
		var=grZ;
		gyro_z=var;
		printf("Z= ");
		print16(&var);
		printf("  ");
		
		var=accX;
		accel_x=var;
		printf("accX= ");
		print16(&var);
		printf("  ");
		var=accY;
		accel_y=var;
		printf("accY= ");
		print16(&var);
		printf("  ");
		var=accZ;
		accel_z=var;
		printf("accZ= ");
		print16(&var);
		printf("  ");
		printf("\n");
		/*--------end------*/
		
		#else
		accel_x=accX;
		accel_y=accY;
		accel_z=accZ;
		gyro_x=grX;
		gyro_y=grY;
		gyro_z=grZ;
		//Gyro angle calculations
		//0.0000611 = 1 / (250Hz / 65.5)
		angle_pitch += gyro_x * 0.0000611; //Calculate the traveled pitch angle and add this to the angle_pitch variable
		angle_roll += gyro_y * 0.0000611;  //Calculate the traveled roll angle and add this to the angle_roll variable
		
		  //0.000001066 = 0.0000611 * (3.142(PI) / 180degr) The Arduino sin function is in radians
		angle_pitch += angle_roll * sin(gyro_z * 0.000001066);               //If the IMU has yawed transfer the roll angle to the pitch angel
		angle_roll -= angle_pitch * sin(gyro_z * 0.000001066);               //If the IMU has yawed transfer the pitch angle to the roll angel
		
		uint16_t lol=angle_pitch;
		uint16_t lol1=angle_roll;
		print16(lol);
		printf(" ");
		print16ln(lol1);
		
		_delay_ms(10);	
		#endif  
	}
	return 0;
}

void mpu6050_calibrate_gyro(int32_t *x,int32_t *y, int32_t *z)
{
	printf("Calibrating Gyro...\n");
	for (int i=0;i<calibration_counter;i++)
	{
		(*x)+=mpu6050_read_gyroX();
		(*y)+=mpu6050_read_gyroY();
		(*z)+=mpu6050_read_gyroZ();
		_delay_ms(caliberation_wait_delay);
	}
		if(*x<0)
		(*x)=abs(*x)/calibration_counter;
		else
		(*x)=(*x)/calibration_counter;
		
		if(*y<0)
		(*y)=abs(*y)/calibration_counter;
		else
		(*y)=(*y)/calibration_counter;
		
		if(*z<0)
		(*z)=abs(*z)/calibration_counter;
		else
		(*z)=(*z)/calibration_counter;
	/*
	//------------do not delete----------
	printf("gyro offsets are");
	print16(x);
	printf("  ");
	print16(y);
	printf("  ");
	print16ln(z);
	_delay_ms(1000);
	//-----------------------------------
	*/			
	
}

void mpu6050_calibrate_accel(int32_t *x,int32_t *y, int32_t *z)
{
	
	printf("Calibrating Accelerometer...\n");
	for(int i=0;i<calibration_counter;i++){
		(*x)+=mpu6050_read_accelX();
		(*y)+=mpu6050_read_accelY();
		(*z)+=mpu6050_read_accelZ();
		_delay_ms(caliberation_wait_delay);
	}
	if(*x<0)
	(*x)=abs(*x)/calibration_counter;
	else
	(*x)=(*x)/calibration_counter;
	
	if(*y<0)
	(*y)=abs(*y)/calibration_counter;
	else
	(*y)=(*y)/calibration_counter;
	
	if(*z<0)
	(*z)=abs(*z)/calibration_counter;
	else
	(*z)=(*z)/calibration_counter;
	
	/*
	//------------do not delete----------
	printf("accel offsets are");
	print16(x);
	printf("  ");
	print16(y);
	printf("  ");
	print16ln(z);
	_delay_ms(1000);
	//-----------------------------------
	*/
}
uint16_t mpu6050_read_gyroX()//axis x=0,y=1,z=2
{
	uint16_t axis_value=mpu6050_rb(MPU6050_RA_GYRO_XOUT_H);
	axis_value=(axis_value<<8) | mpu6050_rb(MPU6050_RA_GYRO_XOUT_L);
	return axis_value;
}
uint16_t mpu6050_read_gyroY()//axis x=0,y=1,z=2
{
	uint16_t axis_value=mpu6050_rb(MPU6050_RA_GYRO_YOUT_H);
	axis_value=(axis_value<<8) | mpu6050_rb(MPU6050_RA_GYRO_YOUT_L);
	return axis_value;
}
uint16_t mpu6050_read_gyroZ()//axis x=0,y=1,z=2
{
	uint16_t axis_value=mpu6050_rb(MPU6050_RA_GYRO_ZOUT_H);
	axis_value=(axis_value<<8) | mpu6050_rb(MPU6050_RA_GYRO_ZOUT_L);
	return axis_value;
}
uint16_t mpu6050_read_accelX()
{
	uint16_t axis_value=mpu6050_rb(MPU6050_RA_ACCEL_XOUT_H);
	axis_value=(axis_value<<8) | mpu6050_rb(MPU6050_RA_ACCEL_XOUT_L);
	return axis_value;
}
uint16_t mpu6050_read_accelY()
{
	uint16_t axis_value=mpu6050_rb(MPU6050_RA_ACCEL_YOUT_H);
	axis_value=(axis_value<<8) | mpu6050_rb(MPU6050_RA_ACCEL_YOUT_L);
	return axis_value;
}
uint16_t mpu6050_read_accelZ()
{
	uint16_t axis_value=mpu6050_rb(MPU6050_RA_ACCEL_ZOUT_H);
	axis_value=(axis_value<<8) | mpu6050_rb(MPU6050_RA_ACCEL_ZOUT_L);
	return axis_value;
}
uint8_t mpu6050_rb(uint8_t byteToRead)//readbyte
{
	i2c_start(MPU6050_WRITE);
	i2c_write(byteToRead);
	i2c_stop();
	
	i2c_start(MPU6050_READ);
	uint8_t byteToReturn=i2c_readNak();
	i2c_stop();
	return byteToReturn;
}
void mpu6050_wb(uint8_t wereToWrite,uint8_t byteToWrite)//writebyte
{
	i2c_start(MPU6050_WRITE);
	i2c_write(wereToWrite);
	i2c_write(byteToWrite);
	i2c_stop();
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

