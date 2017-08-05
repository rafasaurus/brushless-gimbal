
#include "defines.h"
#include "functions.h"
#include "USART.h"
#include "ADC.h"
#include "TIMER.h"
#include "mpu6050registers.h"
#include "MPU6050.h"
#include "KALMAN.h"
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
/*-----------------------------------start of main----------------------------------*/
int main(void)
{	
	U_step=U_step_predefine;
	V_step=V_step_predefine;
	W_step=W_step_predefine;
	incr=-1;
	pwm_delay=2000;
	cli();
	init_gpio();
		#ifdef GYRO
			i2c_init();
		#endif
	USART_Init(MY_UBRR);
	uart_str = fdevopen(uart_putchar, NULL);
	setup_timer0();
	Enable_timer0_overflow_interrupt();//micros
	setup_timer4();//pwm
	setup_timer5();
	Enable_timer5_compare_interrupt();//motor
	OCR5A=4000;
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
	/*----------------------end mpu definition ----------------------*/
	
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
	Kalman_init();
	double angle_roll_kalman=0;
	double roll  = atan2(accel_y, accel_z) * RAD_TO_DEG;
	angle=roll;//set starting angle
	sei();
    while (1) /*---------------------------while(1)---------------------------------*/
    {
		#ifdef GYRO
    		mpu6050_getRawData(&accel_x,&accel_y,&accel_z,&gyro_x,&gyro_y,&gyro_z);//15us to do
			accel_x-=accelX_calib;
			accel_y-=accelY_calib;
			accel_z-=accelZ_calib;
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
			
			double dt = ((double)(micros() - timer1))/1000000;
			timer1=micros();
			double gyroXrate = gyro_x/65.5;// deg/s 
			double gyroYrate = gyro_y/65.5;// deg/s
			if (!loop_bool) {
				angle_pitch += gyroXrate*dt; //Calculate the traveled pitch angle and add this to the angle_pitch variable
				angle_roll += gyroYrate*dt;  //Calculate the traveled roll angle and add this to the angle_roll variable			
			}
			else loop_bool=false;	
			angle_pitch += angle_roll * sin(gyro_z * (dt/65.5*pi/180));               //If the IMU has yawed transfer the roll angle to the pitch angel
			angle_roll -= angle_pitch * sin(gyro_z * (dt/65.5*pi/180));               //If the IMU has yawed transfer the pitch angle to the roll angel
			
			double temporar_accel_x=accel_x/100;
			double temporar_accel_y=accel_y/100;
			double temporar_accel_z=accel_z/100;	
			acc_total_vector = sqrt((temporar_accel_x*temporar_accel_x)+(temporar_accel_y*temporar_accel_y)+(temporar_accel_z*temporar_accel_z));  //Calculate the total accelerometer vector
			acc_total_vector*=100;
		    angle_pitch_acc = asin((double)accel_y/acc_total_vector)* 57.296;       //Calculate the pitch angle
			angle_roll_acc = asin((double)accel_x/acc_total_vector)* -57.296;       //Calculate the roll angle
				
			//kalman
			double roll  = atan2(accel_y, accel_z) * RAD_TO_DEG;
			double pitch = atan(-accel_x / sqrt(accel_y * accel_y + accel_z * accel_z)) * RAD_TO_DEG;
			//angle=roll;
			float kalman_angle=getAngle(angle_roll,gyroYrate,dt);
			
			int16_t reg;
			reg=kalman_angle;
			printSI("",reg);
			reg=angle_roll;
			printSI("",reg);
				
			double xff=angle_roll_acc;
			double final_angleY=(angle_roll*0.996)+(xff*0.004);
			reg=final_angleY;
			printSI("",reg);
			printf("\n");		
			#ifdef DRV8313
				int absoulute_y=abs(kalman_angle);
				uint16_t learing_rate=500;				
				uint16_t local_motor_delay=(32735-(absoulute_y*learing_rate));
				if (local_motor_delay>2000)
				{
					pwm_delay=local_motor_delay;
				}
				int16_t reg_ = local_motor_delay;
				//printSI("ocr=",reg_);
				if ((absoulute_y<=0.8) || (final_angleY >90))
				{
					incr=0;
					//printf("\n");	
				}
				else 
					if (final_angleY>0.8)
					{
						cli();
						incr=1;
						printf(" ");
						int16_t val=pwmSin[U_step];
						//print16(&val);
						//printf(" yes\n");					
						sei();
					}
					else
					{	cli();
						incr=-1;
						printf(" ");
						int16_t val=pwmSin[U_step];
						//print16(&val);
						//printf(" no\n");
						sei();
					}					
			#endif	//DRV8313					 
			//if(set_gyro_angles){                                                 //If the IMU is already started
			//	 angle_pitch = angle_pitch * 0.9996 + angle_pitch_acc * 0.0004;     //Correct the drift of the gyro pitch angle with the accelerometer pitch angle
			//	 angle_roll = angle_roll * 0.9996 + angle_roll_acc * 0.0004;        //Correct the drift of the gyro roll angle with the accelerometer roll angle
			//}
			//else{                                                                //At first start
			//	 angle_pitch = angle_pitch_acc;                                     //Set the gyro pitch angle equal to the accelerometer pitch angle
			//	 angle_roll = angle_roll_acc;                                       //Set the gyro roll angle equal to the accelerometer roll angle
			//	 set_gyro_angles = true;                                            //Set the IMU started flag
			//}			
			#endif //PRINT_RAW_DATA			
		#endif //GYRO
	}
	return 0;
}