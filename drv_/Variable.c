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
//double pid_p_roll=0;
double pid_i_roll=0;
//double pid_d_roll=0;
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
	//float pid_roll_const=15;
	kp_roll=0.02;//0.115;//
	ki_roll=0.001;//0.00062
	kd_roll=0.18;//.015;//0.0095;//
}*/
/*
void Init_Pid_Roll_Vars()
{
	float pid_roll_const=0.118;
	kp_roll=0.659*pid_roll_const;//0.115;//609
	ki_roll=0.00262*pid_roll_const;//0.00062
	kd_roll=0.015*pid_roll_const;//.015;//0.0095;//
}*/


/*
void Init_Pid_Roll_Vars()
{
	double pid_roll_const=0.13;
	kp_roll=0.709*pid_roll_const;//0.115;//609
	ki_roll=0.00062*pid_roll_const;//0.00062
	kd_roll=0.008*pid_roll_const;//.015;//0.0095;//
}*/

//void Init_Pid_Roll_Vars()
//{
//	double pid_roll_const=1;
//	kp_roll=0.1*pid_roll_const;//0.115;//609
//	ki_roll=0*pid_roll_const;//0.00062
//	kd_roll=0.05*pid_roll_const;//.015;//0.0095;//
//}
void Init_Pid_Roll_Vars()//23:28 table size 22*180
{
	double pid_roll_const=0.1;
	kp_roll=11.8*pid_roll_const;//0.115;//609
	ki_roll=0.006*pid_roll_const;//.002*pid_roll_const;//0.00062
	kd_roll=0.0119*pid_roll_const;//.015;//0.0095;//
}
//void Init_Pid_Roll_Vars()//23:25 //text document
//{
//double pid_roll_const=0.1;
//kp_roll=11.3*pid_roll_const;//0.115;//609
//ki_roll=0.0058*pid_roll_const;//.002*pid_roll_const;//0.00062
//kd_roll=0.015*pid_roll_const;//.015;//0.0095;//
//}

////mariage with levon 4:27
//void Init_Pid_Roll_Vars()
//{
//	kp_roll=1.1;//0.115;//
//	ki_roll=0.0062;//0.00062
//	kd_roll=0.00075;//.015;//0.0095;//
//}
//void Init_Pid_Roll_Vars()//levon 4:58
//{
//	double pid_roll_const=0.1;
//	kp_roll=15*pid_roll_const;//0.115;//609  //14
//	ki_roll=0.003*pid_roll_const;//.002*pid_roll_const;//0.00062
//	kd_roll=0.00400015*pid_roll_const;//.015;//0.0095;//
//}
//pupush tver
/*
0.609*pid_roll_const;//0.115;//609
0.00262*pid_roll_const;//0.00062
0.034*pid_roll_const;//.015;//0.0095;//*/
//Init_Pid_Pitch_Vars();














//double error_pitch;
double previous_error_pitch;
//double pid_p_pitch=0;//should be zero
double pid_i_pitch=0;
//double pid_d_pitch=0;

//------------------PID pitch CONSTANTS------------------
double kp_pitch=0.3;//3.55
double ki_pitch=0;//0.003
double kd_pitch=0;//0.001;//2.05
double desired_angle_pitch = 0;


void Init_Pid_Pitch_Vars()
{
	double pop = 0.0005;
	kp_pitch=31*pop;//0.031;//609
	ki_pitch=0.5*pop;//.0002;//0.0005
	kd_pitch=141*pop;
}
/*------------------------PID init end------------------------*/




double dt;
double kalman_angle_roll;
double kalman_angle_pitch;

