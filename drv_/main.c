//#include "Init.h"
//#define PRINT_RAW_DATA
#define GYRO

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


void print16(uint16_t *value);
void print16ln(uint16_t *value);
char *itoa__ (int __val, char *__s, int __radix); 

#define _4millis 400
uint32_t micros_return_value=0;
volatile uint32_t _10micros=0;
uint32_t millis_counter=0;
uint32_t timer1;

FILE * uart_str;
typedef int bool;
enum { false, true };
bool __ftoa(double val, char * buf, int nLen,uint8_t after_decimal_point);//convert double to char
static int uart_putchar(char c, FILE *stream);

//---------------------------------------------

uint16_t ADC_value=0;
uint8_t phase_state=1;//global state 1,2,3,4,5,6
uint8_t reverse=0;
uint8_t com=0;
uint8_t ADC_set_max=0;
uint16_t ADC_max=0;
uint8_t buffer[14];
uint8_t flag=0;
int main(void)

{	
	init_gpio();
	i2c_init();
	USART_Init(MY_UBRR);
	uart_str = fdevopen(uart_putchar, NULL);
	setup_timer3();
	Enable_timer3_compare_interrupt();
	OCR3A=159;//interrupt every 10us
	
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
		mpu6050_calibrate_accel(&accelX_calib,&accelY_calib,&accelZ_calib);
	#endif
	mpu6050_writeByte(MPU6050_RA_SMPLRT_DIV,7);
	mpu6050_writeByte(MPU6050_RA_CONFIG,0x00);
	mpu6050_writeByte(MPU6050_RA_GYRO_CONFIG,0x08);//gyro sensitivity set to 500 o/s
	mpu6050_writeByte(MPU6050_RA_ACCEL_CONFIG,0x10);//accel sensitivity -/+ 8g
	mpu6050_writeByte(MPU6050_RA_PWR_MGMT_1,0x01);
	
	/*-----------------end---------------*/
	sei();
    while (1) 
    {
    	mpu6050_getRawData(&accel_x,&accel_y,&accel_z,&gyro_x,&gyro_y,&gyro_z);//15us to do 
			
			
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
			
			//printf("read= ");
			//printf("  ");
			printf("\n");
			/*--------end------*/
			
		#else
			#ifdef GYRO
							//Gyro angle calculations
				//0.0000611 = 1 / (250Hz / 65.5)
				angle_pitch += gyro_x * 0.0000611; //Calculate the traveled pitch angle and add this to the angle_pitch variable
				angle_roll += gyro_y * 0.0000611;  //Calculate the traveled roll angle and add this to the angle_roll variable
				uint16_t reg=(uint16_t)angle_pitch;
				printf("x= ");
				print16(&gyro_x);
				printf("  ");
				printf("reg= ");
				print16(&reg);
				printf("  \n");
				
				  /*//0.000001066 = 0.0000611 * (3.142(PI) / 180degr) The Arduino sin function is in radians
				angle_pitch += angle_roll * sin(gyro_z * 0.000001066);               //If the IMU has yawed transfer the roll angle to the pitch angel
				angle_roll -= angle_pitch * sin(gyro_z * 0.000001066);               //If the IMU has yawed transfer the pitch angle to the roll angel*/
				
				/* //Accelerometer angle calculations
				 acc_total_vector = sqrt((accel_x*accel_x)+(accel_y*accel_y)+(accel_z*accel_z));  //Calculate the total accelerometer vector
				 //57.296 = 1 / (3.142 / 180) The Arduino asin function is in radians
				 angle_pitch_acc = asin((float)accel_y/acc_total_vector)* 57.296;       //Calculate the pitch angle
				 angle_roll_acc = asin((float)accel_x/acc_total_vector)* -57.296;       //Calculate the roll angle*/
				 
				 if(set_gyro_angles){                                                 //If the IMU is already started
					 angle_pitch = angle_pitch * 0.9996 + angle_pitch_acc * 0.0004;     //Correct the drift of the gyro pitch angle with the accelerometer pitch angle
					 angle_roll = angle_roll * 0.9996 + angle_roll_acc * 0.0004;        //Correct the drift of the gyro roll angle with the accelerometer roll angle
				 }
				 else{                                                                //At first start
					 angle_pitch = angle_pitch_acc;                                     //Set the gyro pitch angle equal to the accelerometer pitch angle
					 angle_roll = angle_roll_acc;                                       //Set the gyro roll angle equal to the accelerometer roll angle
					 set_gyro_angles = true;                                            //Set the IMU started flag
				 }
				  //To dampen the pitch and roll angles a complementary filter is used
				 double var1 = var1 * 0.9 + angle_pitch * 0.1;   //Take 90% of the output pitch value and add 10% of the raw pitch value
				 double var2 = var2 * 0.9 + angle_roll * 0.1;      //Take 90% of the output roll value and add 10% of the raw roll value
				 uint16_t var_to_print1=var1;
				 uint16_t var_to_print2=var2;
				//printf("pitch=");
				//print16(&var_to_print1);
				//printf("  ");
				//
				//printf("roll=");
				//print16(&var_to_print2);
				//printf("\n");
			#endif		
			//USART_Transmit(0xff);
			timer1=_10micros;
			while(_10micros-timer1<_4millis);
		timer1=_10micros;
		#endif  
	}
	return 0;
}
ISR(TIMER3_COMPA_vect)//10 microsecconed timer interrupt
{
		++_10micros;
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
