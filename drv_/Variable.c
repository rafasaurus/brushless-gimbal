/*
 * Variable.c
 *
 * Created: 9/24/2017 4:36:57 PM
 *  Author: rafa
 */ 
#include "Variable.h"
double PID_roll=0;
double PID_pitch=0;
/*---------------------------PID_INIT-------------------------*/
double error_roll=0, previous_error_roll=0;
double pid_i_roll=0;
//------------------PID roll CONSTANTS-------------------

double kp_roll;//=0.169*pid_roll_const;//0.115;//
double ki_roll;//=0.00062*pid_roll_const;//
double kd_roll;//=0.154*pid_roll_const;//.015;//0.0095;//
double desired_angle_roll = 0;

void Init_Pid_Roll_Vars()//23:25 sep 28 //text document  the most stable
{
	kp_roll=1.1;//0.115;//609
	ki_roll=0.000045;//.00069;//.002*pid_roll_const;//0.00062
	kd_roll=0.105;//.000009;//.015;//0.0095;//
}

double previous_error_pitch;
double pid_i_pitch=0;

//------------------PID pitch CONSTANTS------------------
double kp_pitch=0.3;//3.55
double ki_pitch=0;//0.003
double kd_pitch=0;//0.001;//2.05
double desired_angle_pitch = 0;


void Init_Pid_Pitch_Vars()
{
	double pop = 0.1;
	kp_pitch=10.5*pop;//0.031;//609
	ki_pitch=0.21*pop;//.0002;//0.0005
	kd_pitch=0.54*pop;
}
/*------------------------PID init end------------------------*/

double dt;
double kalman_angle_roll;
double kalman_angle_pitch;

