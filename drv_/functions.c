#include "functions.h"
#include "Variable.h"
int U_step=U_step_predefine;
int V_step=V_step_predefine;
int W_step=W_step_predefine;
int U_step_2=U_step_predefine;
int V_step_2=V_step_predefine;
int W_step_2=W_step_predefine;
int U_step_3=U_step_predefine;
int V_step_3=V_step_predefine;
int W_step_3=W_step_predefine;


#if SINPRESCALER==1
uint8_t pwmSin[]={128,131,135,138,141,145,148,151,155,158,161,
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
#elif SINPRESCALER==2
uint8_t pwmSin[]={64,65,67,69,70,72,73,75,77,78,80,82,83,85,87,
	88,90,91,93,95,96,98,99,101,102,104,105,107,108,110,111,112,
	112,113,113,114,114,114,115,115,115,116,116,116,117,117,117,
	117,118,118,118,118,118,118,118,118,119,119,119,119,119,119,
	119,119,119,118,118,118,118,118,118,118,118,117,117,117,117,
	116,116,116,115,115,115,114,114,114,113,113,112,112,111,112,
	112,113,113,114,114,114,115,115,115,116,116,116,117,117,117,
	117,118,118,118,118,118,118,118,118,119,119,119,119,119,119,
	119,119,119,118,118,118,118,118,118,118,118,117,117,117,117,
	116,116,116,115,115,115,114,114,114,113,113,112,112,111,110,
	108,107,105,104,102,101,99,98,96,95,93,91,90,88,87,85,83,82,
	80,78,77,75,73,72,70,69,67,65,64,62,60,58,57,55,54,52,50,49,
	47,45,44,42,40,39,37,36,34,32,31,29,28,26,25,23,22,20,19,17,
	16,15,15,14,14,13,13,13,12,12,12,11,11,11,10,10,10,10,9,9,9,
	9,9,9,9,9,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,10,10,10,10,11,
	11,11,12,12,12,13,13,13,14,14,15,15,16,15,15,14,14,13,13,13,
	12,12,12,11,11,11,10,10,10,10,9,9,9,9,9,9,9,9,8,8,8,8,8,8,8,
	8,8,9,9,9,9,9,9,9,9,10,10,10,10,11,11,11,12,12,12,13,13,13,14,
	14,15,15,16,17,19,20,22,23,25,26,28,29,31,32,34,36,37,39,40,
	42,44,45,47,49,50,52,54,55,57,58,60,62,64
};
#elif SINPRESCALER==3
uint8_t pwmSin[]={128,131,135,138,141,145,148,151,155,158,161,164,
	168,171,174,177,181,184,187,190,193,197,200,203,206,209,212,215,
	218,221,224,225,225,226,227,228,229,230,230,231,232,232,233,234
	,234,235,235,236,236,236,237,237,237,238,238,238,238,238,238,238
	,238,238,238,238,238,238,238,238,237,237,237,236,236,236,235,235
	,234,234,233,232,232,231,230,230,229,228,227,226,225,225,224,225
	,225,226,227,228,229,230,230,231,232,232,233,234,234,235,235,236
	,236,236,237,237,237,238,238,238,238,238,238,238,238,238,238,238
	,238,238,238,238,237,237,237,236,236,236,235,235,234,234,233,232
	,232,231,230,230,229,228,227,226,225,225,224,221,218,215,212,209
	,206,203,200,197,193,190,187,184,181,177,174,171,168,164,161,158
	,155,151,148,145,141,138,135,131,128,125,121,118,115,111,108,105
	,101,98,95,92,88,85,82,79,75,72,69,66,63,59,56,53,50,47,44,41,38
	,35,32,31,31,30,29,28,27,26,26,25,24,24,23,22,22,21,21,20,20,20
	,19,19,19,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,19,19,19
	,20,20,20,21,21,22,22,23,24,24,25,26,26,27,28,29,30,31,31,32,31
	,31,30,29,28,27,26,26,25,24,24,23,22,22,21,21,20,20,20,19,19,19
	,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,19,19,19,20,20,20
	,21,21,22,22,23,24,24,25,26,26,27,28,29,30,31,31,32,35,38,41,44
	,47,50,53,56,59,63,66,69,72,75,79,82,85,88,92,95,98,101,105,108
	,111,115,118,121,125,128};
#elif SINPRESCALER==4

#endif
void init_gpio()
{
	
}
void getSinTable(uint16_t sinTableSize,uint8_t *pwmSin,uint16_t sineScale)
{
		for (int i = 0; i < sinTableSize; i++)
		{
			double x = i * (2 * pi) / sinTableSize;
			pwmSin[i] = (sin(x) * sineScale) + sineScale;
			//uint16_t reg=pwmSin[i];	
			//print16(&reg);
			printf("\n");
		}
}

double min (double a,double b, double c)
{
	if (a<b && a<c) return a;
	if (b<a && b<c) return b;
	if (c<a && c<b) return c;
	return a;
}
double max (double a,double b, double c)
{	
	if (a>b && a>c) return a;
	if (b>a && b>c) return b;
	if (c>a && c>b) return c;
	return a;
}
void PWM_update()//motor pwm update
{
	PID_roll=Compute_PID(kalman_angle_roll, 0 , &pid_i_roll, &previous_error_roll,dt,kp_roll,ki_roll,kd_roll);

	//if (abs(kalman_angle_roll)<0.1 || abs(kalman_angle_roll>75))
	////if (0)
	//{
	//	incr=0;
	//	//pid_i_roll=0;
	//}
	//else
	//PID_roll = map(PID_roll,0,1000,0,255);
	if (abs(kalman_angle_roll)<44)
	{
		uint16_t pwm_PID=25000;
// 		if (abs(PID_roll)>255)
// 		PID_roll=255;
		//if (abs(kalman_angle_roll)<0.1)
		//	pid_i_roll = 0;
		if (kalman_angle_roll>0)
		{
			//printf("yes");
			
			incr = (uint8_t)(abs(PID_roll));
			pwm_PID -=150*incr;
		}
		else
		{
			incr = -(uint8_t)(abs(PID_roll));
			pwm_PID -=-150*incr;
			//printf("no");
		}
		U_PWM=pwmSin[U_step];
		V_PWM=pwmSin[V_step];
		W_PWM=pwmSin[W_step];
		INT_MOTOR_SPEED=pwm_PID;
	}
	
	
	//U_PWM=85;
	//V_PWM=170;
	//W_PWM=255;
	U_step=U_step+incr;  //370 u -10
	V_step=V_step+incr;
	W_step=W_step+incr;
	if(U_step > SINE_TABLE_SZ)
	U_step-=SINE_TABLE_SZ ;
	if(U_step < 0)
	U_step=SINE_TABLE_SZ+U_step;
	
	if(V_step > SINE_TABLE_SZ)
	V_step-=SINE_TABLE_SZ;
	if(V_step < 0)
	V_step=SINE_TABLE_SZ+V_step;
	
	if(W_step > SINE_TABLE_SZ)
	W_step-=SINE_TABLE_SZ;
	if(W_step < 0)
	W_step=SINE_TABLE_SZ+W_step;
	//printf("debug 0000000");

}
void PWM_update_2()//motor pwm update
{
	PID_pitch=Compute_PID(kalman_angle_pitch, 0 ,&pid_i_pitch,&previous_error_pitch,dt,kp_pitch,ki_pitch,kd_pitch);
	//printSD("pid_pitch ",PID_pitch);
	/*if (abs(kalman_angle_pitch)<0.1|| abs(kalman_angle_pitch>75))//45
	//if(0)
	{
		incr_2=0;
		//pid_i_pitch=0;
	}
	else*/
	
		if (kalman_angle_pitch>0)
		//if(1)
		{
			incr_2=(uint8_t)(abs(PID_pitch));
			//printSI("in ",incr_2);
		}
		else
		{
			incr_2=-(uint8_t)(abs(PID_pitch));
			//printSI("in ",incr_2);
			//printf("no");
		}
	//U1_PWM=pwmSin[U_step_2];
	//V1_PWM=pwmSin[V_step_2];
	//W1_PWM=pwmSin[W_step_2];
	uint16_t pop=2400;
	U1_PWM=pwmSin[pop];
	V1_PWM=pwmSin[pop+V_step_predefine];
	W1_PWM=pwmSin[pop+W_step_predefine];
	
	U_step_2=U_step_2+incr_2;
	V_step_2=V_step_2+incr_2;
	W_step_2=W_step_2+incr_2;
	if(U_step_2 > SINE_TABLE_SZ)
	U_step_2-=SINE_TABLE_SZ ;
	if(U_step_2 < 0)
	U_step_2=SINE_TABLE_SZ+U_step_2;
	
	if(V_step_2 > SINE_TABLE_SZ)
	V_step_2-=SINE_TABLE_SZ;
	if(V_step_2 < 0)
	V_step_2=SINE_TABLE_SZ+V_step_2;
	
	if(W_step_2 > SINE_TABLE_SZ)
	W_step_2-=SINE_TABLE_SZ;
	if(W_step_2 < 0)
	W_step_2=SINE_TABLE_SZ+W_step_2;
	//_delay_us(100);
	
	//printf("oopooo");
}
/*
void PWM_update_3()
{
	//PID_roll=Compute_PID(kalman_angle_roll, 0 , &pid_i_roll, &previous_error_roll,dt,kp_roll,ki_roll,kd_roll);
	//
	//if (abs(kalman_angle_roll)<0.1 || abs(kalman_angle_roll>75))
	//{
	//	incr=0;
	//	pid_i_roll=0;
	//}
	//else
	//if (kalman_angle_roll>0.1)
	//{
	//	//printf("yes");
	//	incr = (uint8_t)(abs(PID_roll));
	//}
	//else
	//{
	//	incr = -(uint8_t)(abs(PID_roll));
	//	//printf("no");
	//}
	//U2_PWM=pwmSin[U_step_3];
	//V2_PWM=pwmSin[V_step_3];
	//W2_PWM=pwmSin[W_step_3];
	U2_PWM=512;
	V2_PWM=521;
	W2_PWM=512;
	U_step_3=U_step_3+incr_3;
	V_step_3=V_step_3+incr_3;
	W_step_3=W_step_3+incr_3;
	if(U_step_3 > SINE_TABLE_SZ)
	U_step_3 = 0;
	if(U_step_3 < 0)
	U_step_3 = SINE_TABLE_SZ;
	
	if(V_step_3 > SINE_TABLE_SZ)
	V_step_3 = 0;
	if(V_step_3 < 0)
	V_step_3 = SINE_TABLE_SZ;
	
	if(W_step_3 > SINE_TABLE_SZ)
	W_step_3 = 0;
	if(W_step_3 < 0)
	W_step_3 = SINE_TABLE_SZ;
	//printf("debug 0000000");	
}*/
void init_motor_gpio()
{
	//setting pin mode to output for motor 1 
	sbi(DDRH,3);//digital 6 OC4A
	sbi(DDRH,4);//digital 7 OC4B
	sbi(DDRH,5);//digital 8 OC4C
	
	//setting pin mode to output for motor 2
	sbi(DDRE,4);//digital 2 OC3B
	sbi(DDRE,5);//digital 3 OC3C
	sbi(DDRE,3);//digital 5 OC3A
	
	//setting pin mode to output for motor 3
	//timer5
	sbi(DDRL,3);
	sbi(DDRL,4);
	sbi(DDRL,5);
}