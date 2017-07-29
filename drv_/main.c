//#include "Init.h"
//#define PRINT_RAW_DATA
#define GYRO
#define DRV8313
#define SIN_TABLE_PRESCALER 1
#define BITSPERLONG 32
#define TOP2BITS(x) ((x & (3L << (BITSPERLONG-2))) >> (BITSPERLONG-2))

struct int_sqrt {
	unsigned sqrt, frac;
};

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
uint32_t SquareRoot(uint32_t a_nInput);
void PWM_update();
void print16(uint16_t *value);
void print16ln(uint16_t *value);
char *itoa__ (int __val, char *__s, int __radix); 

#define _4millis 400
uint32_t micros_return_value=0;
volatile uint32_t _10micros=0;
uint32_t millis_counter=0;

FILE * uart_str;
typedef int bool;
enum { false, true };
bool __ftoa(double val, char * buf, int nLen,uint8_t after_decimal_point);//convert double to char
static int uart_putchar(char c, FILE *stream);
void writeOcr(uint16_t bla);
//---------------------------------------------

uint16_t ADC_value=0;
uint8_t phase_state=1;//global state 1,2,3,4,5,6
uint8_t reverse=0;
uint8_t com=0;
uint8_t ADC_set_max=0;
uint16_t ADC_max=0;
uint8_t buffer[14];
uint8_t flag=0;
/*
const uint8_t pwmSin[] = {128, 147, 166, 185, 203, 221, 238, 243, 248, 251, 253, 255, 255,
	 255, 253, 251, 248, 243, 238, 243, 248, 251, 253, 255, 255, 255, 253, 251, 248, 243,
	  238, 221, 203, 185, 166, 147, 128, 109, 90, 71, 53, 35, 18, 13, 8, 5, 3, 1, 1, 1,
	   3, 5, 8, 13, 18, 13, 8, 5, 3, 1, 1, 1, 3, 5, 8, 13, 18, 35, 53, 71, 90, 109};*/
	   
/*
const uint8_t pwmSin[] = {128, 132, 136, 140, 143, 147, 151, 155, 159, 162, 166, 170,
	 174, 178, 181, 185, 189, 192, 196, 200, 203, 207, 211, 214, 218, 221, 225, 228, 
	 232, 235, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 248, 249, 250, 
	 250, 251, 252, 252, 253, 253, 253, 254, 254, 254, 255, 255, 255, 255, 255, 255,
	  255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 253, 252, 252, 251,
	   250, 250, 249, 248, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 239,
	    240, 241, 242, 243, 244, 245, 246, 247, 248, 248, 249, 250, 250, 251, 252, 252,
		 253, 253, 253, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		  255, 255, 255, 254, 254, 254, 253, 253, 253, 252, 252, 251, 250, 250, 249, 248,
		   248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 235, 232, 228, 225, 221,
		    218, 214, 211, 207, 203, 200, 196, 192, 189, 185, 181, 178, 174, 170, 166, 162,
			 159, 155, 151, 147, 143, 140, 136, 132, 128, 124, 120, 116, 113, 109, 105, 101,
			  97, 94, 90, 86, 82, 78, 75, 71, 67, 64, 60, 56, 53, 49, 45, 42, 38, 35, 31, 28,
			   24, 21, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 8, 7, 6, 6, 5, 4, 4, 3, 3, 3,
			    2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 6, 6, 
				7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9,
				 8, 8, 7, 6, 6, 5, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
				  2, 2, 3, 3, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 21, 24,
				   28, 31, 35, 38, 42, 45, 49, 53, 56, 60, 64, 67, 71, 75, 78, 82, 86, 90, 94, 97, 101,
				    105, 109, 113, 116, 120, 124};*/
#if SIN_TABLE_PRESCALER==1
const uint8_t pwmSin[]={
	127,130,133,136,139,143,146,149,152,155,158,161,164,167,170,173,176,178,
	181,184,187,190,192,195,198,200,203,205,208,210,212,215,217,219,221,223,
	225,227,229,231,233,234,236,238,239,240,242,243,244,245,247,248,249,249,
	250,251,252,252,253,253,253,254,254,254,254,254,254,254,253,253,253,252,
	252,251,250,249,249,248,247,245,244,243,242,240,239,238,236,234,233,231,
	229,227,225,223,221,219,217,215,212,210,208,205,203,200,198,195,192,190,
	187,184,181,178,176,173,170,167,164,161,158,155,152,149,146,143,139,136,
	133,130,127,124,121,118,115,111,108,105,102,99,96,93,90,87,84,81,78,76,
	73,70,67,64,62,59,56,54,51,49,46,44,42,39,37,35,33,31,29,27,25,23,21,20,
	18,16,15,14,12,11,10,9,7,6,5,5,4,3,2,2,1,1,1,0,0,0,0,0,0,0,1,1,1,2,2,3,4,
	5,5,6,7,9,10,11,12,14,15,16,18,20,21,23,25,27,29,31,33,35,37,39,42,44,46,
	49,51,54,56,59,62,64,67,70,73,76,78,81,84,87,90,93,96,99,102,105,108,111,
	115,118,121,124
	};
#elif SIN_TABLE_PRESCALER==2
const uint8_t pwmSin[]={63,65,66,68,69,71,73,74,76,77,79,80,82,83,85,86,88,89,90
	,92,93,95,96,97,99,100,101,102,104,105,106,107,108,109,110,111,112,113,114,115
	,116,117,118,119,119,120,121,121,122,122,123,124,124,124,125,125,126,126,126,126
	,126,127,127,127,127,127,127,127,126,126,126,126,126,125,125,124,124,124,123,122
	,122,121,121,120,119,119,118,117,116,115,114,113,112,111,110,109,108,107,106,105
	,104,102,101,100,99,97,96,95,93,92,90,89,88,86,85,83,82,80,79,77,76,74,73,71,69
	,68,66,65,63,62,60,59,57,55,54,52,51,49,48,46,45,43,42,40,39,38,36,35,33,32,31
	,29,28,27,25,24,23,22,21,19,18,17,16,15,14,13,12,11,10,10,9,8,7,7,6,5,5,4,3,3
	,2,2,2,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,2,2,2,3,3,4,5,5,6,7,7,8,9,10,10
	,11,12,13,14,15,16,17,18,19,21,22,23,24,25,27,28,29,31,32,33,35,36,38,39,40
	,42,43,45,46,48,49,51,52,54,55,57,59,60,62
	};
	#elif SIN_TABLE_PRESCALER==3
const uint8_t pwmSin[]={42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62
	,63,64,65,66,66,67,68,69,70,70,71,72,73,73,74,75,75,76,77,77,78,78,79,79,80,80,81
	,81,81,82,82,83,83,83,83,84,84,84,84,84,84,84,84,84,84,84,84,84,84,84,84,84,83,83
	,83,83,82,82,81,81,81,80,80,79,79,78,78,77,77,76,75,75,74,73,73,72,71,70,70,69,68
	,67,66,66,65,64,63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42
	,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,18,17,16
	,15,14,14,13,12,11,11,10,9,9,8,7,7,6,6,5,5,4,4,3,3,3,2,2,1,1,1,1,0,0,0,0,0,0,0,0
	,0,0,0,0,0,0,0,0,0,1,1,1,1,2,2,3,3,3,4,4,5,5,6,6,7,7,8,9,9,10,11,11,12,13,14,14,15
	,16,17,18,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41
	};
	#elif SIN_TABLE_PRESCALER==4
const uint8_t pwmSin[]={31,32,33,34,34,35,36,37,38,38,39,40,41,41,42,43,44,44,45,46,46
	,47,48,48,49,50,50,51,52,52,53,53,54,54,55,55,56,56,57,57,58,58,59,59,59,60,60,60
	,61,61,61,62,62,62,62,62,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,62,62
	,62,62,62,61,61,61,60,60,60,59,59,59,58,58,57,57,56,56,55,55,54,54,53,53,52,52,51
	,50,50,49,48,48,47,46,46,45,44,44,43,42,41,41,40,39,38,38,37,36,35,34,34,33,32,31
	,31,30,29,28,27,27,26,25,24,24,23,22,21,21,20,19,19,18,17,16,16,15,14,14,13,12,12
	,11,11,10,9,9,8,8,7,7,6,6,5,5,5,4,4,3,3,3,2,2,2,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0
	,0,0,0,0,0,0,0,1,1,1,1,1,2,2,2,3,3,3,4,4,5,5,5,6,6,7,7,8,8,9,9,10,11,11,12,12,13
	,14,14,15,16,16,17,18,19,19,20,21,21,22,23,24,24,25,26,27,27,28,29,30,31
	};
	#elif SIN_TABLE_PRESCALER==5
const uint8_t pwmSin[]={25,26,26,27,27,28,29,29,30,31,31,32,32,33,34,34,35,35,36,36,37
	,38,38,39,39,40,40,41,41,42,42,43,43,43,44,44,45,45,45,46,46,46,47,47,47,48,48,48
	,48,49,49,49,49,49,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50
	,49,49,49,49,49,48,48,48,48,47,47,47,46,46,46,45,45,45,44,44,43,43,43,42,42,41,41
	,40,40,39,39,38,38,37,36,36,35,35,34,34,33,32,32,31,31,30,29,29,28,27,27,26,26,25
	,24,24,23,23,22,21,21,20,19,19,18,18,17,16,16,15,15,14,14,13,12,12,11,11,10,10,9
	,9,8,8,7,7,7,6,6,5,5,5,4,4,4,3,3,3,2,2,2,2,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	,0,0,0,0,0,0,0,1,1,1,1,1,2,2,2,2,3,3,3,4,4,4,5,5,5,6,6,7,7,7,8,8,9,9,10,10,11,11
	,12,12,13,14,14,15,15,16,16,17,18,18,19,19,20,21,21,22,23,23,24,24
	};
#endif
bool direction;
int incr=-1;//increment variable
uint8_t sinTableSize = 255;//sizeof(pwmSin)/sizeof(int); // Find lookup table size
uint8_t phase = 60;//sinTableSize / 3;         // Find phase shift and initial A, B C phase values
int U_step=0;
int V_step=85;
int W_step=170;
uint16_t pwm_delay=0;
int main(void)

{	
	
	init_gpio();
	i2c_init();
	USART_Init(MY_UBRR);
	uart_str = fdevopen(uart_putchar, NULL);
	setup_timer1();
	setup_timer3();
	setup_timer4();//pwm
	Enable_timer3_compare_interrupt();
	Enable_timer1_compare_interrupt();
	OCR3A=159;//interrupt every 10us
	OCR5A=4000;
	
	//Counter top value. Freq = 16 MHz/prescaler/(OCR0A + 1)
	//ADC_Init();
	//setup_timer0();
	//Enable_timer0_compare_interrupt();
	//init_gpio();
	
	//sbi(ADCSRA,ADSC);
	
	/*----------MPU6050 twi init---------*/
	
	int16_t gyro_x;
	int16_t gyro_y;
	int16_t gyro_z;
	int16_t accel_x;
	int16_t accel_y;
	int16_t accel_z;
	float angle_pitch=0;
	float angle_roll=0;
	float acc_total_vector=0;
	float angle_pitch_acc=0;
	float angle_roll_acc=0;
	bool set_gyro_angles;
	#ifdef CALIBERATED_DATA
		int32_t gyroX_calib=0;
		int32_t gyroY_calib=0;
		int32_t gyroZ_calib=0;
		int32_t accelX_calib=0;
		int32_t accelY_calib=0;
		int32_t accelZ_calib=0;
		mpu6050_calibrate_gyro(&gyroX_calib,&gyroY_calib,&gyroZ_calib);
		//mpu6050_calibrate_accel(&accelX_calib,&accelY_calib,&accelZ_calib);
	#endif
	mpu6050_writeByte(MPU6050_RA_SMPLRT_DIV,7);
	mpu6050_writeByte(MPU6050_RA_CONFIG,0x00);
	mpu6050_writeByte(MPU6050_RA_GYRO_CONFIG,0x08);//gyro sensitivity set to 500 o/s
	mpu6050_writeByte(MPU6050_RA_ACCEL_CONFIG,0x10);//accel sensitivity -/+ 8g
	mpu6050_writeByte(MPU6050_RA_PWR_MGMT_1,0x01);
	
	/*-----------------end---------------*/
	sei();
	uint32_t timer1=_10micros;
    while (1) 
    {
    	mpu6050_getRawData(&accel_x,&accel_y,&accel_z,&gyro_x,&gyro_y,&gyro_z);//15us to do
    	double dt = (double)((_10micros - timer1));
    	timer1 = _10micros;
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
			
			//printf("x= ");
			//print16(&gyro_x);
			//printf("  ");
			//
			//printf("y= ");
			//print16(&gyro_y);
			//printf("  ");
			//
			//printf("Z= ");
			//print16(&gyro_z);
			//printf("  ");
			
			
			printf("accX= ");
			print16(&accel_x);
			printf("  ");
			
			printf("accY= ");
			print16(&accel_y);
			printf("  ");
			
			printf("accZ= ");
			print16(&accel_z);
			printf("  ");
			
			//printf("read= ");
			//printf("  ");
			printf("\n");
			/*--------end------*/
			
		#else
			#ifdef GYRO
				//Accelerometer angle calculations
				double gyroXrate = gyro_x / 65.5; // Convert to deg/s
				double gyroYrate = gyro_y / 65.5; // Convert to deg/s
				angle_pitch += gyroXrate*dt/100000; //Calculate the traveled pitch angle and add this to the angle_pitch variable
				angle_roll += gyroYrate*dt/100000;  //Calculate the traveled roll angle and add this to the angle_roll variable
				
				//0.000001066 = 0.0000611 * (3.142(PI) / 180degr) The Arduino sin function is in radians
				angle_pitch += angle_roll * sin(gyro_z * 0.000001066);               //If the IMU has yawed transfer the roll angle to the pitch angel
				angle_roll -= angle_pitch * sin(gyro_z * 0.000001066);               //If the IMU has yawed transfer the pitch angle to the roll angel
				 
				acc_total_vector = sqrt((accel_x*accel_x)+(accel_y*accel_y)+(accel_z*accel_z));  //Calculate the total accelerometer vector
				//57.296 = 1 / (3.142 / 180) The Arduino asin function is in radians
				angle_pitch_acc = asin((float)accel_y/acc_total_vector)* 57.296;       //Calculate the pitch angle
				angle_roll_acc = asin((float)accel_x/acc_total_vector)* -57.296;       //Calculate the roll angle
				//double acc_total_vector_=sqrt((169*169)+(129*129)+(4308*4308));
				
				//long double acc_total_vector_=SquareRoot((long double)(4308*4308));
				//printf("x=");
				//print16(&gyro_x);
				//uint16_t reg=angle_pitch;
				////printf(" ");
				//printf("angle_x= ");
				//print16(&reg);
				////printf(" ");
				////printf("y=");
				////print16(&gyro_y);
				//reg=angle_roll;
				////printf(" ");
				//printf(" angle_y=");
				//print16(&reg);
				//reg=dt;
				//printf(" ");
				//printf("dt= ");
				//print16(&reg);
				
				//jul28 debug
				////accel print
				/*printf("accX= ");
				print16(&accel_x);
				printf("  ");
				
				printf("accY= ");
				print16(&accel_y);
				printf("  ");
				
				printf("accZ= ");
				print16(&accel_z);
				printf("  ");
				uint16_t reg=acc_total_vector;
				printf(" ");
				printf("accel= ");
				print16ln(&reg);*/
				
				#ifdef DRV8313
					int reg=angle_pitch;
					//printf(" ");
					printf("angle_x= ");
					print16(&reg);
					printf(" ");
					uint16_t local_motor_delay=(32735-abs(reg)*500);
					if (local_motor_delay>2000)
					{
						pwm_delay=2*local_motor_delay;
					}
					uint16_t reg_ =local_motor_delay;
					printf("ocr= ");
					print16(&reg_);
					if (reg==0)
					{
						incr=0;
						printf("\n");	
					}
					else if (reg>0)
						{
							cli();
							incr=1;
							printf(" yes\n");
							
							sei();
						}
						else
						{	cli();
							incr=-1;
							printf(" no\n");
							sei();
						}
					
				#endif
				
				
				 
				//if(set_gyro_angles){                                                 //If the IMU is already started
				//	 angle_pitch = angle_pitch * 0.9996 + angle_pitch_acc * 0.0004;     //Correct the drift of the gyro pitch angle with the accelerometer pitch angle
				//	 angle_roll = angle_roll * 0.9996 + angle_roll_acc * 0.0004;        //Correct the drift of the gyro roll angle with the accelerometer roll angle
				//}
				//else{                                                                //At first start
				//	 angle_pitch = angle_pitch_acc;                                     //Set the gyro pitch angle equal to the accelerometer pitch angle
				//	 angle_roll = angle_roll_acc;                                       //Set the gyro roll angle equal to the accelerometer roll angle
				//	 set_gyro_angles = true;                                            //Set the IMU started flag
				//}
				// //To dampen the pitch and roll angles a complementary filter is used
				//double var1 = var1 * 0.9 + angle_pitch * 0.1;   //Take 90% of the output pitch value and add 10% of the raw pitch value
				//double var2 = var2 * 0.9 + angle_roll * 0.1;      //Take 90% of the output roll value and add 10% of the raw roll value
				//uint16_t var_to_print1=var1;
				//uint16_t var_to_print2=var2;
				//printf("pitch=");
				//print16(&var_to_print1);
				//printf("  ");
				//
				//printf("roll=");
				//print16(&var_to_print2);
				//printf("\n");
			#endif		
			
			//////for (int i=0;i<1000;++i);
			////while(_10micros-timer1<_4millis)
			////{
			////	//uint16_t pop = (_10micros-timer1)*10;
			////	//print16ln(&pop);
			////}
			
			
		#endif  
	}
	return 0;
}
ISR(TIMER3_COMPA_vect)//10 microsecconed timer interrupt
{
		++_10micros;
		//HS_U_INVERSE;
		
}
ISR(TIMER5_COMPA_vect)//10 microsecconed timer interrupt
{
	PWM_update();
	//PWM_update();
}
void PWM_update()
{
	U_PWM=pwmSin[U_step];
	V_PWM=pwmSin[V_step];
	W_PWM=pwmSin[W_step];
	U_step=U_step+incr;
	V_step=V_step+incr;
	W_step=W_step+incr;
	if(U_step > sinTableSize)  U_step = 0;
	if(U_step < 0)  U_step = sinTableSize;
	
	if(V_step > sinTableSize)  V_step = 0;
	if(V_step < 0)  V_step = sinTableSize;
	
	if(W_step > sinTableSize)  W_step = 0;
	if(W_step < 0) W_step = sinTableSize;
	//_delay_us(100);
	OCR5A=pwm_delay;
}
void writeOcr(uint16_t bla)
{	
	cli();
	OCR1A=bla;
	sei();
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
uint32_t SquareRoot(uint32_t a_nInput)
{
	uint32_t op  = a_nInput;
	uint32_t res = 0;
	uint32_t one = 1uL << 30; // The second-to-top bit is set: use 1u << 14 for uint16_t type; use 1uL<<30 for uint32_t type


	// "one" starts at the highest power of four <= than the argument.
	while (one > op)
	{
		one >>= 2;
	}

	while (one != 0)
	{
		if (op >= res + one)
		{
			op = op - (res + one);
			res = res +  2 * one;
		}
		res >>= 1;
		one >>= 2;
	}
	return res;
}