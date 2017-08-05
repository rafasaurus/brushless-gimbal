//#define PRINT_RAW_DATA
#define GYRO
#define CALIBERATED_DATA
#define DRV8313
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
		#endif
		#ifdef GYRO
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
			
			printf("read= ");
			printf("  ");
			printf("\n");
			/*--------end------*/			
		#else
			
			double dt = (double)((micros() - timer1));
			timer1=micros();
			//double hz=1000000/dt;
			double pop=1/(65.5*1000000/dt);
			double gyroXrate = gyro_x*pop;// / 65.5 / hz; 
			double gyroYrate = gyro_y*pop;// / 65.5 / hz; 
			//double tpel=gyroYrate;
			////print16ln(&tpel);
			//if(loop_bool)
			//{
			//	gyroYrate-=tpel;
			//	loop_bool=false;
			//}
			if (!loop_bool)
			{
				angle_pitch += gyroXrate; //Calculate the traveled pitch angle and add this to the angle_pitch variable
				angle_roll += gyroYrate;  //Calculate the traveled roll angle and add this to the angle_roll variable			
			}
			else
			{
				loop_bool=false;
			}					
					//0.000001066 = 0.0000611 * (3.142(PI) / 180degr) The Arduino sin function is in radians
			pop*=(3.142/180);	
			angle_pitch += angle_roll * sin(gyro_z * pop);               //If the IMU has yawed transfer the roll angle to the pitch angel
			angle_roll -= angle_pitch * sin(gyro_z * pop);               //If the IMU has yawed transfer the pitch angle to the roll angel
			
			double temporar_accel_x=accel_x/100;
			double temporar_accel_y=accel_y/100;
			double temporar_accel_z=accel_z/100;
			
			acc_total_vector = sqrt((temporar_accel_x*temporar_accel_x)+(temporar_accel_y*temporar_accel_y)+(temporar_accel_z*temporar_accel_z));  //Calculate the total accelerometer vector
					//57.296 = 1 / (3.142 / 180) The Arduino asin function is in radians
			acc_total_vector*=100;
		    angle_pitch_acc = asin((double)accel_y/acc_total_vector)* 57.296;       //Calculate the pitch angle
			angle_roll_acc = asin((double)accel_x/acc_total_vector)* -57.296;       //Calculate the roll angle
				
			#ifdef COMPANGLE//comp noise reduction algorithm 
				double roll  = atan2(accel_y, accel_z) * RAD_TO_DEG;
				double pitch = atan(-accel_x / sqrt(accel_y * accel_y + accel_z * accel_z)) * RAD_TO_DEG;
				compAngleX=roll;
				double gyroXrate_ = gyro_x / 65.5; // Convert to deg/s
				double gyroYrate_ = gyro_y / 65.5; // Convert to deg/s	
				gyroXangle += gyroXrate_ * dt; // Calculate gyro angle without any filter
				gyroYangle += gyroYrate_ * dt;
				compAngleX = 0.93 * (compAngleX + gyroXrate * dt) + 0.07 * roll; // Calculate the angle using a Complimentary filter
				compAngleY = 0.93 * (compAngleY + gyroYrate * dt) + 0.07 * pitch;
				
				
				
			#endif
			//kalman
			double roll  = atan2(accel_y, accel_z) * RAD_TO_DEG;
			//angle=roll;//setangle	
			float kalman_dt=dt/1000000;
			double kalman_gyroyRate=gyro_y/65.5;
			float kalman_angle=getAngle(roll,kalman_gyroyRate,kalman_dt);
			//angle_roll_kalman+=pop*kalman_angle;	
			
			
			
			
			int16_t reg=kalman_angle;
			printf(" ");
			printf("kalman_angle= ");
			print16(&reg);
			reg=angle_roll;
			printf(" ");
			printf("gyroY_angle= ");
			print16(&reg);
			
			double xff=angle_roll_acc;
			double final_angleY=(angle_roll*0.996)+(xff*0.004);
			reg=final_angleY;
			printf(" ");	
			printf("final_angleY= ");
			print16(&reg);
						
			#ifdef DRV8313
				int absoulute_y=abs(final_angleY);
				uint16_t learing_rate=500;				
				uint16_t local_motor_delay=(32735-(absoulute_y*learing_rate));
				if (local_motor_delay>2000)
				{
					pwm_delay=local_motor_delay;
				}
				int16_t reg_ = local_motor_delay;
				printf("  ");
				printf("ocr= ");
				print16(&reg_);
				if ((absoulute_y<=0.8) || (final_angleY >90))
				{
					incr=0;
					printf("\n");	
				}
				else 
					if (final_angleY>0.8)
					{
						cli();
						incr=1;
						printf(" ");
						int16_t val=pwmSin[U_step];
						print16(&val);
						printf(" yes\n");
						
						sei();
					}
					else
					{	cli();
						incr=-1;
						printf(" ");
						int16_t val=pwmSin[U_step];
						print16(&val);
						printf(" no\n");
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