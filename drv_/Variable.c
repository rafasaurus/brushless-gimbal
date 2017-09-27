/*
 * Variable.c
 *
 * Created: 9/24/2017 4:36:57 PM
 *  Author: rafja
 */ 
#include "Variable.h"
double PID_roll=0;
double PID_pitch=0;
/*---------------------------PID_INIT-------------------------*/
double error_roll=0, previous_error_roll=0;
double pid_p_roll=0;
double pid_i_roll=0;
double pid_d_roll=0;
//------------------PID roll CONSTANTS-------------------
//first time tweak values kp=0.5 kd=0.03 ki=0
//seccond time tweak values kp=0.41 kd=0.007 ki=0

double kp_roll;//=0.169*pid_roll_const;//0.115;//
double ki_roll;//=0.00062*pid_roll_const;//
double kd_roll;//=0.154*pid_roll_const;//.015;//0.0095;//
double desired_angle_roll = 0;


/*
void Init_Pid_Roll_Vars()
{
	float pid_roll_const=0.118;
	kp_roll=0.169*pid_roll_const;//0.115;//
	ki_roll=0.00062*pid_roll_const;//0.00062
	kd_roll=0.154*pid_roll_const;//.015;//0.0095;//
}*/
/*
void Init_Pid_Roll_Vars()
{
	float pid_roll_const=0.118;
	kp_roll=0.659*pid_roll_const;//0.115;//609
	ki_roll=0.00262*pid_roll_const;//0.00062
	kd_roll=0.015*pid_roll_const;//.015;//0.0095;//
}*/
void Init_Pid_Roll_Vars()
{
	double pid_roll_const=0.118;
	kp_roll=0.659*pid_roll_const;//0.115;//609
	ki_roll=0.00262*pid_roll_const;//0.00062
	kd_roll=0.015*pid_roll_const;//.015;//0.0095;//
}

//pupush tver
/*
0.609*pid_roll_const;//0.115;//609
0.00262*pid_roll_const;//0.00062
0.034*pid_roll_const;//.015;//0.0095;//*/
//Init_Pid_Pitch_Vars();














double error_pitch, previous_error_pitch;
double pid_p_pitch=0;//should be zero
double pid_i_pitch=0;
double pid_d_pitch=0;

//------------------PID pitch CONSTANTS------------------
double kp_pitch=0.3;//3.55
double ki_pitch=0;//0.003
double kd_pitch=0;//0.001;//2.05
double desired_angle_pitch = 0;


void Init_Pid_Pitch_Vars()
{
	double pid_pitch_const=0.068;
	kp_pitch=0.031*pid_pitch_const;//0.031;//609
	ki_pitch=0.0005*pid_pitch_const;//0.0005
	kd_pitch=0.141*pid_pitch_const;//.141;//0.0095;//
}
/*------------------------PID init end------------------------*/




double dt;
double kalman_angle_roll;
double kalman_angle_pitch;

