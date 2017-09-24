/*
 * PID.h
 *
 * Created: 9/24/2017 10:52:23 AM
 *  Author: rafja
 */ 


#ifndef PID_H_
#define PID_H_

#include <avr/io.h>
#include "defines.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "USART.h"
double Compute_PID(double angle, double desired_angle,double *pid_i,double *previous_error, double dt,double kp, double ki, double kd);

#endif /* PID_H_ */