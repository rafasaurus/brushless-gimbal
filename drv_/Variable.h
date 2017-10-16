/*
 * Variable.h
 *
 * Created: 9/24/2017 10:55:14 AM
 *  Author: rafa
 */ 
#ifndef VARIABLE_H_
#define VARIABLE_H_
extern double PID_roll;
extern double PID_pitch;
extern double kalman_angle_roll;
extern double kalman_angle_pitch;
/*---------------------------PID_INIT-------------------------*/
//extern double error_roll;
extern double previous_error_roll;
//extern double pid_p_roll;
extern double pid_i_roll;
//extern double pid_d_roll;
//------------------PID roll CONSTANTS-------------------
extern double kp_roll;
extern double ki_roll;
extern double kd_roll;
extern double desired_angle_roll;

//extern double error_pitch;
extern double previous_error_pitch;
//extern double pid_p_pitch;
extern double pid_i_pitch;
//extern double pid_d_pitch;
//------------------PID pitch CONSTANTS------------------
extern double kp_pitch;
extern double ki_pitch;
extern double kd_pitch;
extern double desired_angle_pitch;
extern double dt;
/*------------------------PID init end------------------------*/
void Init_Pid_Roll_Vars();
void Init_Pid_Pitch_Vars();
#endif /* VARIABLE_H_ */