//check pwm_delay
/*
------------------some notes------------------
This code is written for 2-axis gimbal, but 3rd axis driver exist,
if you want to turn z-axis on see function.c file PWM_update_3() function.

This software drives 3 axis brushless gimbal using DRV8313 drivers,
and Atmel AVR Atmega2560 microcontroller.
This software uses Kalman filter for data filtering of mpu6050 accelerometer
and gyroscope using i2c(TWI) interface.
For controlling brushles DC motors software uses sinusoidal tables,
this can be generated on start of the program, or precalculated in microsoft excel,
or other software, see function.c file.
The code also uses PID controller for each axis.	
*/
#include "Variable.h"
#include "defines.h"
#include "functions.h"
#include "USART.h"
#include "ADC.h"
#include "TIMER.h"
#include "mpu6050registers.h"
#include "MPU6050.h"
#include "KALMAN.h"
#include "PID.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

uint8_t buffer[14];
bool loop_bool=true;
unsigned char receiveData;
unsigned char receiveData_[20];
/*-----------------------------------start of main----------------------------------*/
int main(void)
{	
	//predefines motor(s) phases, by 120 degree comutation, see defines.h
	U_step=U_step_predefine;
	V_step=V_step_predefine;
	W_step=W_step_predefine;
	U_step_2=U_step_predefine;
	V_step_2=V_step_predefine;
	W_step_2=W_step_predefine;
	cli();//disable interrupts
	init_motor_gpio();
	#ifdef GYRO
		i2c_init();
	#endif
	USART_Init(MY_UBRR);
	uart_str = fdevopen(uart_putchar, NULL);
	setup_timer0();
	Enable_timer0_overflow_interrupt();//micros
	setup_timer4();//pwm for motor1
	setup_timer3();//pwm for motor2
	setup_timer5();//pwm for motor3
	setup_timer1();
	Enable_timer1_compare_interrupt();//motor 1
	U2_PWM=85-25;//motor 3
	V2_PWM=170-25;
	W2_PWM=255-25;
	Init_Pid_Roll_Vars();
	Init_Pid_Pitch_Vars();
	MOTORS_INTERRUPT_SPEED=25000;//this is a motor(s) update interrupt 
	unsigned long timer1=micros();//for while loop timer
	/*----------MPU6050 twi init---------*/
	#ifdef GYRO
		int16_t gyro_x;//for reading gyro accel datas
		int16_t gyro_y;
		int16_t gyro_z;
		int16_t accel_x;
		int16_t accel_y;
		int16_t accel_z;
			#ifdef CALIBERATED_DATA//enables auto calibration on start
				int32_t gyroX_calib_=0;
				int32_t gyroY_calib_=0;
				int32_t gyroZ_calib_=0; 				
				mpu6050_calibrate_gyro(&gyroX_calib_,&gyroY_calib_,&gyroZ_calib_);
				int16_t gyroX_calib=gyroX_calib_;
				int16_t gyroY_calib=gyroY_calib_;
				int16_t gyroZ_calib=gyroZ_calib_;
			#else
				int16_t gyroX_calib=gyro_offset_x;
				int16_t gyroY_calib=gyro_offset_y;
				int16_t gyroZ_calib=gyro_offset_z;
			#endif
		mpu6050_writeByte(MPU6050_RA_SMPLRT_DIV,7);
		mpu6050_writeByte(MPU6050_RA_CONFIG,0x00);
		mpu6050_writeByte(MPU6050_RA_GYRO_CONFIG,0x08);//gyro sensitivity set to 500 o/s
		mpu6050_writeByte(MPU6050_RA_ACCEL_CONFIG,0x10);//accel sensitivity -/+ 8g
		mpu6050_writeByte(MPU6050_RA_PWR_MGMT_1,0x01);
	#endif

	/*----------------------------motor init-------------------------*/
	#ifdef GENERATE_SIN
		getSinTable(SINE_TABLE_SZ,pwmSin,sinScale);
		printf("U_step_predefine="); print16ln(&U_step);
		printf("  V_step_predefine="); print16ln(&V_step);
		printf("  W_step_predefine="); print16ln(&W_step);  
	#endif 
	#ifdef COMPANGLE
		double gyroXangle;
		double gyroYangle;
		double compAngleX;
		double compAngleY;
	#endif  
	/*---------------------------kalman_init----------------------*/
	Kalman_init();
	Kalman_init_1();
	_delay_ms(100);
	double roll  = atan2(accel_y, accel_z) * RAD_TO_DEG;
	double pitch =  atan2(accel_x, sqrt(accel_y*accel_y + accel_z*accel_z)) * RAD_TO_DEG;
	angle=0;//global angle for pi
	angle_1=0;		
	sei();//enable interrupts
	/*---------------------------while(1)---------------------------------*/
    while (1) 
    {	
		#ifdef GYRO
    		mpu6050_getRawData(&accel_x,&accel_y,&accel_z,&gyro_x,&gyro_y,&gyro_z);//15us to do
			gyro_x-=gyroX_calib;
			gyro_y-=gyroY_calib;
			gyro_z-=gyroZ_calib;
			dt = ((double)(micros() - timer1))/1000000;//calculated time of while loop
			timer1=micros();
			double gyroXrate = gyro_x/65.5;// deg/s 
			double gyroYrate = gyro_y/65.5;
			//-------------------------------NEW accel-----------------
			double temporar_accel_y=accel_y/100;
			double temporar_accel_z=accel_z/100;
			double temporar_var=sqrt(temporar_accel_y*temporar_accel_y + temporar_accel_z*temporar_accel_z);
			temporar_var*=100;
			pitch = atan2(accel_x, temporar_var) * RAD_TO_DEG;	
			roll  = atan2(accel_y, accel_z) * RAD_TO_DEG;		
			/*-------------------------Kalman--------------------------------*/		
			kalman_angle_roll=getAngle(roll,gyroXrate,dt);
			kalman_angle_pitch=getAngle_1(pitch,gyroYrate,dt);
			/*------------------------Debug Print----------------------------*/
			printSD("kro = ",kalman_angle_roll);
		#endif //GYRO
	}
	return 0;
}