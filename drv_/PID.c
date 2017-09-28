/*
 * PID.c
 *
 * Created: 9/24/2017 10:52:39 AM
 *  Author: rafja
 */ 
#include "PID.h"
#include "Variable.h"

double Compute_PID(double angle, double desired_angle,double *pid_i,double *previous_error, double dt,double kp, double ki, double kd)
{
	//printSD("prever_local = ", *previous_error);

	double error = desired_angle-angle;
	double pid_p = kp*error;
	//if (abs(error)>0.1)
	//if (abs(error)<10/kp_roll)
	//{
		*pid_i=*pid_i+(ki*error);	
		//
	//}
	
	
	double pid_d = kd*((error - *previous_error)/dt);
	*previous_error=error;
	double PID_FINAL= pid_p + (*pid_i) + pid_d;
	if (PID_FINAL<PID_MAX)
		return PID_FINAL;
	else return PID_MAX;
}
