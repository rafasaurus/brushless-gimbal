//#define ENABLE_PID_LIB
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
	//variableInit();
	U_step=U_step_predefine;
	V_step=V_step_predefine;
	W_step=W_step_predefine;
	U_step_2=U_step_predefine;
	V_step_2=V_step_predefine;
	W_step_2=W_step_predefine;
	incr=-1;
	incr_2=-1;
	pwm_delay=25000;
	pwm_delay_2=32000;
	cli();
	init_gpio();
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
	setup_timer5();
	Enable_timer5_compare_interrupt();//motor 1
	setup_timer1();
	//Enable_timer1_compare_interrupt();//motor 2
	INT_MOTOR_SPEED1=pwm_delay;
	INT_MOTOR_SPEED2=pwm_delay_2;
	unsigned long timer1=micros();
	/*----------MPU6050 twi init---------*/
	#ifdef GYRO
		int16_t gyro_x;
		int16_t gyro_y;
		int16_t gyro_z;
		int16_t accel_x;
		int16_t accel_y;
		int16_t accel_z;
		double angle_pitch=0;
		double angle_roll=0;
		double acc_total_vector=0;
		double angle_pitch_acc=0;
		double angle_roll_acc=0;
		bool set_gyro_angles=false;
			#ifdef CALIBERATED_DATA
				int32_t gyroX_calib_=0;
				int32_t gyroY_calib_=0;
				int32_t gyroZ_calib_=0;
				int32_t accelX_calib_=0;
				int32_t accelY_calib_=0;
				int32_t accelZ_calib_=0;				
				//uint16_t gyroX_angle_calib=0; 
				uint16_t gyroY_angle_calib=0; 
				//uint16_t gyroZ_angle_calib=0; 				
				mpu6050_calibrate_gyro(&gyroX_calib_,&gyroY_calib_,&gyroZ_calib_);
				int16_t gyroX_calib=gyroX_calib_;
				int16_t gyroY_calib=gyroY_calib_;
				int16_t gyroZ_calib=gyroZ_calib_;
				int16_t accelX_calib=accelX_calib_;
				int16_t accelY_calib=accelY_calib_;
				int16_t accelZ_calib=accelZ_calib_;				
				//mpu6050_calibrate_accel(&accelX_calib,&accelY_calib,&accelZ_calib);
			#else
				int16_t gyroX_calib=gyro_offset_x;
				int16_t gyroY_calib=gyro_offset_y;
				int16_t gyroZ_calib=gyro_offset_z;
				int16_t accelX_calib=accel_offset_x;
				int16_t accelY_calib=accel_offset_y;
				int16_t accelZ_calib=accel_offset_z;
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
	double pitch =  atan2(accel_x, sqrt(accel_y*accel_y + accel_z*accel_z)) * RAD_TO_DEG;// atan(-accel_x / sqrt(accel_y * accel_y + accel_z * accel_z)) * RAD_TO_DEG;
	angle=0;
	angle_1=0;		
	sei();
	
    while (1) /*---------------------------while(1)---------------------------------*/
    {	
		#ifdef GYRO
    		mpu6050_getRawData(&accel_x,&accel_y,&accel_z,&gyro_x,&gyro_y,&gyro_z);//15us to do
			//accel_x-=accelX_calib;
			//accel_y-=accelY_calib;
			//accel_z-=accelZ_calib;
			gyro_x-=gyroX_calib;
			gyro_y-=gyroY_calib;
			gyro_z-=gyroZ_calib;
		#ifdef PRINT_RAW_DATA
			/*--------raw data gyro-accel------*/
			printSI("gx=",gyro_x);
			printSI("gy=",gyro_x);
			printSI("gz=",gyro_x);
			printSI("ax=",accel_x);
			printSI("ay=",accel_y);
			printSI("az=",accel_z);
			printf("\n");
			/*--------end------*/			
		#else
			dt = ((double)(micros() - timer1))/1000000;
			timer1=micros();
			double gyroXrate = gyro_x/65.5;// deg/s 
			double gyroYrate = gyro_y/65.5;// deg/s
			if (!loop_bool) {
				angle_roll += gyroXrate*dt; //Calculate the traveled pitch angle and add this to the angle_pitch variable
				angle_pitch += gyroYrate*dt;  //Calculate the traveled roll angle and add this to the angle_roll variable			
			}
			else loop_bool=false;	
			//angle_pitch += angle_roll * sin(gyro_z * (dt/65.5*pi/180));               //If the IMU has yawed transfer the roll angle to the pitch angel
			//angle_roll -= angle_pitch * sin(gyro_z * (dt/65.5*pi/180));
			//If the IMU has yawed transfer the pitch angle to the roll angel
		
			//-------------------------------NEW accel-----------------
			roll  = atan2(accel_y, accel_z) * RAD_TO_DEG;
			double temporar_accel_x=accel_x/100;
			double temporar_accel_y=accel_y/100;
			double temporar_accel_z=accel_z/100;
			double temporar_var=sqrt(temporar_accel_y*temporar_accel_y + temporar_accel_z*temporar_accel_z);
			temporar_var*=100;
			pitch = atan2(accel_x, temporar_var) * RAD_TO_DEG;

						
			/*-------------------------Kalman--------------------------------*/		
			kalman_angle_roll=getAngle(roll,gyroXrate,dt);
			kalman_angle_pitch=getAngle_1(pitch,gyroYrate,dt);
			
			//printSD("kro = ",kalman_angle_roll);
			printSD("kalPitch = ",kalman_angle_pitch);
			////printf("\n");
			//printSD("roll = ",roll);
			printSD("pitch = ",pitch);	
			//printSI("gx=",gyro_x);
			//printSI("gy=",gyro_x);
			//printSI("gz=",gyro_x);
			//printSI("ax=",accel_x);
			//printSI("ay=",accel_y );
			//printSI("az=",accel_z);
				
			//printSD("PID_roll ",PID_roll);
			
			//printSD("PID_pitch ",PID_pitch);
			//printSD("pid_i ",pid_i_roll);
			printf("\n");							
			#endif //PRINT_RAW_DATA			
		#endif //GYRO
	}
	return 0;
}
ISR (USART0_RX_vect)
{
	printf("->");
	receiveData=UDR0;
	//strcat(receiveData,UDR0);
	printf("%s",&receiveData);
	_delay_ms(5000);
}