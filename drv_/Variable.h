/*
 * Variable.h
 *
 * Created: 9/24/2017 10:55:14 AM
 *  Author: rafja
 */ 


#ifndef VARIABLE_H_
#define VARIABLE_H_

extern float PID, error, previous_error;
extern float pid_p;
extern float pid_i;
extern float pid_d;
/////////////////PID CONSTANTS/////////////////
extern double kp;//3.55
extern double ki;//0.003
extern double kd;//2.05

//PID_TEST	
extern double pid_i_new;
extern double previous_error_new;
extern double dt;
extern float kalman_angle_x;
extern float kalman_angle_y;

#endif /* VARIABLE_H_ */