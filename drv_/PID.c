/*
 * PID.c
 *
 * Created: 9/24/2017 10:52:39 AM
 *  Author: rafja
 */ 
#include "PID.h"
double Compute_PID(double angle, double desired_angle,double *pid_i,double *previous_error, double dt,double kp, double ki, double kd)
{
	double error = angle-desired_angle;
	double pid_p = kp*error;
	if (abs(error)  <3)
	{
		*pid_i=*pid_i+(ki*error);
	}
	double pid_d = kd*((error - *previous_error)/dt);
	*previous_error=error;
	
	//printSI("pidi ",*pid_i);
	//printSI("preverr1 ",*previous_error);
	//printSI("dt1 ",dt);
	//printSI("kp1 ",kp );
	//printSI("ki1 ",ki);
	//printSI("kd1 ",kd);
	return pid_p + *pid_i + pid_d;
}
