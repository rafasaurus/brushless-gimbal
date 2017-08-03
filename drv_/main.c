#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)
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
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
void PWM_update(void);
void print16(int16_t *value);
void print16ln(int16_t *value);

/*----------------micros,millis functions---------------*/

unsigned long micros();
unsigned long millis();
//volatile unsigned long timer2_overflow_count=0;
volatile unsigned long timer0_overflow_count = 0;
volatile unsigned long timer0_millis = 0;
static unsigned char timer0_fract = 0;
#define clockCyclesPerMicrosecond() (F_CPU / 1000000L)

FILE * uart_str;
typedef int bool; 
enum { false, true };
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


bool direction;
int incr=-1;//increment variable
uint16_t pwm_delay=2000;
bool loop_bool=true;

int U_step=U_step_predefine;
int V_step=V_step_predefine;
int W_step=W_step_predefine;
#ifdef GENERATE_SIN
	uint8_t pwmSin[SINE_TABLE_SZ];
#else
	uint8_t pwmSin[] ={128,131,135,138,141,145,148,151,155,158,161,
		164,168,171,174,177,181,184,187,190,193,197,200,203,206,209,
		212,215,218,221,224,225,225,226,227,228,229,230,230,231,232,
		232,233,234,234,235,235,236,236,236,237,237,237,238,238,238,
		238,238,238,238,238,238,238,238,238,238,238,238,237,237,237,
		236,236,236,235,235,234,234,233,232,232,231,230,230,229,228,
		227,226,225,225,224,225,225,226,227,228,229,230,230,231,232,
		232,233,234,234,235,235,236,236,236,237,237,237,238,238,238,
		238,238,238,238,238,238,238,238,238,238,238,238,237,237,237,
		236,236,236,235,235,234,234,233,232,232,231,230,230,229,228,
		227,226,225,225,224,221,218,215,212,209,206,203,200,197,193,
		190,187,184,181,177,174,171,168,164,161,158,155,151,148,145,
		141,138,135,131,128,125,121,118,115,111,108,105,101,98,95,92,
		88,85,82,79,75,72,69,66,63,59,56,53,50,47,44,41,38,35,32,31,
		31,30,29,28,27,26,26,25,24,24,23,22,22,21,21,20,20,20,19,19,
		19,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,19,19,19,20,
		20,20,21,21,22,22,23,24,24,25,26,26,27,28,29,30,31,31,32,31,
		31,30,29,28,27,26,26,25,24,24,23,22,22,21,21,20,20,20,19,19,
		19,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,19,19,19,20,
		20,20,21,21,22,22,23,24,24,25,26,26,27,28,29,30,31,31,32,35,
		38,41,44,47,50,53,56,59,63,66,69,72,75,79,82,85,88,92,95,98,
		101,105,108,111,115,118,121,125,128
	};
#endif
//uint8_t pwmSin[] = {128, 132, 136, 140, 143, 147, 151, 155, 159, 162, 166, 170, 174, 178, 181, 185, 189, 192, 196, 200, 203, 207, 211, 214, 218, 221, 225, 228, 232, 235, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 248, 249, 250, 250, 251, 252, 252, 253, 253, 253, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 253, 252, 252, 251, 250, 250, 249, 248, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 248, 249, 250, 250, 251, 252, 252, 253, 253, 253, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 253, 252, 252, 251, 250, 250, 249, 248, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 235, 232, 228, 225, 221, 218, 214, 211, 207, 203, 200, 196, 192, 189, 185, 181, 178, 174, 170, 166, 162, 159, 155, 151, 147, 143, 140, 136, 132, 128, 124, 120, 116, 113, 109, 105, 101, 97, 94, 90, 86, 82, 78, 75, 71, 67, 64, 60, 56, 53, 49, 45, 42, 38, 35, 31, 28, 24, 21, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 8, 7, 6, 6, 5, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 8, 7, 6, 6, 5, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 21, 24, 28, 31, 35, 38, 42, 45, 49, 53, 56, 60, 64, 67, 71, 75, 78, 82, 86, 90, 94, 97, 101, 105, 109, 113, 116, 120, 124};

/*-----------------------------------start of main----------------------------------*/
int main(void)
{				
		
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
	unsigned long timer1;
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
	USART_Transmit(0xfe); 
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
			double gyroXrate = gyro_x*pop;// / 65.5 / hz; // Convert to deg/s
			double gyroYrate = gyro_y*pop;// / 65.5 / hz; // Convert to deg/s
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
					
			uint16_t reg=angle_pitch;
			printf(" ");
			printf("gyroX_angle= ");
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
				uint16_t reg_print=final_angleY;
				int absoulute_y=abs(final_angleY);
				uint16_t learing_rate=5;				
				uint16_t local_motor_delay=(32735-(absoulute_y*learing_rate));
				if (local_motor_delay>2000)
				{
					pwm_delay=local_motor_delay;
				}
				uint16_t reg_ = local_motor_delay;
				printf("  ");
				printf("ocr= ");
				print16(&reg_);
				if (final_angleY>=-0.8 && final_angleY<=0.8  || final_angleY >90)
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
						uint16_t val=pwmSin[U_step];
						print16(&val);
						printf(" yes\n");
						
						sei();
					}
					else
					{	cli();
						incr=-1;
						printf(" ");
						uint16_t val=pwmSin[U_step];
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
ISR(TIMER0_OVF_vect)//10 microsecconed timer interrupt
{
	unsigned long m = timer0_millis;
	unsigned char f = timer0_fract;

	m += MILLIS_INC;
	f += FRACT_INC;
	if (f >= FRACT_MAX) {
		f -= FRACT_MAX;
		m += 1;
	}


	timer0_fract = f;
	timer0_millis = m;
	timer0_overflow_count++;
}
unsigned long micros() {
	unsigned long m;
	uint8_t oldSREG = SREG, t;
	
	cli();
	m = timer0_overflow_count;
	t = TCNT0;

	
	if ((TIFR0 & _BV(TOV0)) && (t < 255))
	m++;

	SREG = oldSREG;
	
	return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
}

unsigned long millis()
{
	unsigned long m;
	uint8_t oldSREG = SREG;

	cli();
	m = timer0_millis;
	SREG = oldSREG;

	return m;
}
ISR(TIMER5_COMPA_vect)//motor update interrupt routine
{
	PWM_update();
}
void PWM_update()//motor pwm update 
{
	U_PWM=pwmSin[U_step];
	V_PWM=pwmSin[V_step];
	W_PWM=pwmSin[W_step];
	U_step=U_step+incr;
	V_step=V_step+incr;
	W_step=W_step+incr;
	if(U_step > SINE_TABLE_SZ)  
		U_step = 0;
	if(U_step < 0)  
		U_step = SINE_TABLE_SZ;
	
	if(V_step > SINE_TABLE_SZ)  
		V_step = 0;
	if(V_step < 0)  
		V_step = SINE_TABLE_SZ;
	
	if(W_step > SINE_TABLE_SZ)  
		W_step = 0;
	if(W_step < 0) 
		W_step = SINE_TABLE_SZ;
	//_delay_us(100);
	OCR5A=pwm_delay;
}
void print16(int16_t *value)
//this is pointer value, transmited value
//must be reference type &
{
	char c[10];
	itoa(*value, c, 10);
	printf(c);
	//printf("\n");
}

void print16ln(int16_t *value)
//this is pointer value, transmited value
//must be reference type &
{
	char c[10];
	itoa(*value, c, 10);
	printf(c);
	printf("\n");
}

static int uart_putchar(char c, FILE *stream)
{
	if (c == '\n')
	uart_putchar('\r', stream);
	while (!( UCSR0A & (1<<UDRE0)));

	UDR0 = c;
	return 0;
}