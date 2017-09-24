/*
 * PID.h
 *
 * Created: 9/24/2017 10:52:23 AM
 *  Author: rafja
 */ 


#ifndef PID_H_
#define PID_H_
#include "Variable.h"
#include "USART.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
double Compute_PID(double angle, double desired_angle,double *pid_i,double *previous_error, double dt,double kp, double ki, double kd);
double Compute_PID_ROLL();

#endif /* PID_H_ */