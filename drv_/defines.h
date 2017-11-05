#ifndef DEFINES
	#define DEFINES
	/*******************global macros****************/
	//#define PRINT_RAW_DATA
	#define GYRO
	//#define CALIBERATED_DATA
	#define DRV8313

	unsigned int					pwm_delay;
	int								pwm_delay_2;
	typedef int bool;
	enum { false, true };
	#define F_CPU					16000000UL
	#define BAUD					250000
	//#define BAUD 57600
	#define MY_UBRR					(F_CPU/16/BAUD-1)
	
	#define cbi						(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
	#define sbi						(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
	#define pinChangeHigh			(reg, pin) (reg|=(1<<pin))
	#define pinChangeLow			(reg, pin)  (reg&=~(1<<pin))
	
	#define MPU6050_WRITE			(0x68<<1) 
	#define MPU6050_READ			((0x68<<1)|1)
	
	#define U_PWM					OCR4A
	#define V_PWM					OCR4B
	#define W_PWM					OCR4C
	#define U1_PWM					OCR3A
	#define V1_PWM					OCR3B
	#define W1_PWM					OCR3C
	#define INT_MOTOR_SPEED1		OCR5A
	#define INT_MOTOR_SPEED2		OCR1A

	#define TIMER5_FREQ				250000//hz
	#define TIMER5_TO_US(OCR_)		((F_CPU/(2*TIMER5_FREQ))-1)
	//_delay_us();
	//EN1 = LS_U					digital 7 PORTD ^= (1<<7);
	//EN2 = LS_V					digital 8 PORTB ^=(1<<0);
	//EN3 = LS_W 					digital 2 PORTD ^=(1<<2);
	//#define						LS_U_HIGH (PORTD|=(1<<7))
	//#define						LS_U_LOW (PORTD&=~(1<<7))
	//
	//#define						LS_V_HIGH (PORTB|=(1<<0))
	//#define						LS_V_LOW (PORTB&=~(1<<0))
	//
	//#define						LS_W_HIGH (PORTD|=(1<<2))
	//#define						LS_W_LOW (PORTD&=~(1<<2))
	
	#define clockCyclesToMicroseconds(a)		(((a) * 1000L) / (F_CPU / 1000L))
	#define MICROSECONDS_PER_TIMER0_OVERFLOW	(clockCyclesToMicroseconds(64 * 256))
	#define MILLIS_INC							(MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)
	// the fractional number of milliseconds per timer0 overflow. we shift right
	// by three to fit these numbers into a byte. (for the clock speeds we care
	// about - 8 and 16 MHz - this doesn't lose precision.)
	#define pi									(3.14159265359)
	#define RAD_TO_DEG							(57.2957786)
	#define FRACT_INC							((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
	#define FRACT_MAX							(1000 >> 3)
	
	//#define GENERATE_SIN
	#ifdef GENERATE_SIN
		#define SINE_TABLE_SZ		720
		#define sinScale			127//this value is mid range value
		#define phase				(SINE_TABLE_SZ/3)
		#define U_step_predefine	0
		#define V_step_predefine	(U_step_predefine+phase)
		#define W_step_predefine	(V_step_predefine+phase)
		//#define SVPWM
	#else
		#define SINPRESCALER		3
		#define SINE_TABLE_SZ		360
		#define sinScale			127//this value is mid range value
		#define phase				(SINE_TABLE_SZ/3)
		#define U_step_predefine	0
		#define V_step_predefine	(U_step_predefine+phase)
		#define W_step_predefine	(V_step_predefine+phase)
		
		#define U1_step_predefine	0
		#define V1_step_predefine	(U_step_predefine+phase)
		#define W1_step_predefine	(V_step_predefine+phase)
	
		#endif

	//Your offsets:	-298	-2782	2134	59	114	311
	#define gyro_offset_x			-108
	#define gyro_offset_y			-224
	#define gyro_offset_z			-625
	#define accel_offset_x			-298
	#define accel_offset_y			-2782
	#define accel_offset_z			2134
	//#define accX (accel_x-=accelX_calib)
	//#define accY (accel_y-=accelY_calib)
	//#define accZ (accel_z-=accelZ_calib)
	//#define grX  (gyro_x-=gyroX_calib)
	//#define grY  (gyro_y-=gyroY_calib)
	//#define grZ  (gyro_z-=gyroZ_calib)
	#define calibration_counter		4000
	#define caliberation_wait_delay 5//in ms
	#define THE_MAIN_OUTPUT			kalman_angle_x
	#define THE_MAIN_OUTPUT_2		kalman_angle_y
	
//	#define MOTOR_2_UPDATE
#endif //DEFINES