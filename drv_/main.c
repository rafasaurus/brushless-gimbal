//#include "Init.h"
//#define PRINT_RAW_DATA
#define GYRO
#define DRV8313

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
				    105, 109, 113, 116, 120, 124};

bool direction;
int incr=-1;//increment variable
uint8_t sinTableSize = 180;//sizeof(pwmSin)/sizeof(int); // Find lookup table size
uint8_t phase = 60;//sinTableSize / 3;         // Find phase shift and initial A, B C phase values
int U_step=0;
int V_step=60;
int W_step=120;
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
	OCR1A=4000;
	
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
					uint16_t reg=angle_pitch;
					//printf(" ");
					printf("angle_x= ");
					print16(&reg);
					printf(" ");
					pwm_delay =abs(reg)*200;
					printf("ocr= ");
					print16ln(&pwm_delay);
					//if (reg>=0)
					//	{
					//		cli();
					//		incr=1;
					//		sei();
					//	}
					//	else
					//	{	cli();
					//		incr=-1;
					//		sei();
					//	}
					
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
ISR(TIMER1_COMPA_vect)//10 microsecconed timer interrupt
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
	OCR1A=pwm_delay;
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