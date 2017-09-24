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
float error_roll=0, previous_error_roll=0;
float pid_p_roll=0;
float pid_i_roll=0;
float pid_d_roll=0;
//------------------PID roll CONSTANTS-------------------
//first time tweak values kp=0.5 kd=0.03 ki=0
//seccond time tweak values kp=0.41 kd=0.007 ki=0

double kp_roll=0.41;//
double kd_roll=0.007;//
double ki_roll=0;//

float desired_angle_roll = 0;

float error_pitch, previous_error_pitch;
float pid_p_pitch=0;
float pid_i_pitch=0;
float pid_d_pitch=0;
//------------------PID pitch CONSTANTS------------------
double kp_pitch=20;//3.55
double ki_pitch=0.24;//0.003
double kd_pitch=26;//2.05
float desired_angle_pitch = 0;
/*------------------------PID init end------------------------*/

double dt;
float kalman_angle_roll;
float kalman_angle_pitch;

