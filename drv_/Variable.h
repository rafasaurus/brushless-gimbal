/*
 * Variable.h
 *
 * Created: 9/24/2017 10:55:14 AM
 *  Author: rafja
 */ 


#ifndef VARIABLE_H_
#define VARIABLE_H_
/*---------------------------PID_INIT-------------------------*/
extern float error_roll, previous_error_roll;
extern float pid_p_roll;
extern float pid_i_roll;
extern float pid_d_roll;
//------------------PID roll CONSTANTS-------------------
extern double kp_roll;
extern double ki_roll;
extern double kd_roll;
extern float desired_angle_roll;

extern float error_pitch, previous_error_pitch;
extern float pid_p_pitch;
extern float pid_i_pitch;
extern float pid_d_pitch;
//------------------PID pitch CONSTANTS------------------
extern double kp_pitch;
extern double ki_pitch;
extern double kd_pitch;
extern float desired_angle_pitch;
/*------------------------PID init end------------------------*/

#endif /* VARIABLE_H_ */