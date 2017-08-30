/*
 * PID.c
 *
 * Created: 8/21/2017 5:09:39 PM
 *  Author: rafa
 */ 
#include "PID.h"

bool PID_Compute()
{
	if(!inAuto) return false;
	unsigned long now = millis();
	unsigned long timeChange = (now - lastTime);
	if(timeChange>=SampleTime)
	{
		/*Compute all the working error variables*/
		double input = *myInput;
		double error = *mySetpoint - input;
		ITerm+= (ki * error);
		if(ITerm > outMax) ITerm= outMax;
		else if(ITerm < outMin) ITerm= outMin;
		double dInput = (input - lastInput);
		
		/*Compute PID Output*/
		double output = kp * error + ITerm- kd * dInput;
		
		if(output > outMax) output = outMax;
		else if(output < outMin) output = outMin;
		*myOutput = output;
		
		/*Remember some variables for next time*/
		lastInput = input;
		lastTime = now;
		return true;
	}
	else return false;
}

PID_SetOutputLimits(double Min, double Max)
{
	if(Min >= Max) return;
	outMin = Min;
	outMax = Max;
	
	if(inAuto)
	{
		if(*myOutput > outMax) *myOutput = outMax;
		else if(*myOutput < outMin) *myOutput = outMin;
		
		if(ITerm > outMax) ITerm= outMax;
		else if(ITerm < outMin) ITerm= outMin;
	}
}
PID_SetMode(int Mode)
{
	bool newAuto = (Mode == AUTOMATIC);
	if(newAuto && !inAuto)
	{  /*we just went from manual to auto*/
		PID_Initialize();
	}
	inAuto = newAuto;
}
PID_Initialize()
{
	ITerm = *myOutput;
	lastInput = *myInput;
	if(ITerm > outMax) ITerm = outMax;
	else if(ITerm < outMin) ITerm = outMin;
}
PID_SetControllerDirection(int Direction)
{
	if(inAuto && Direction !=controllerDirection)
	{
		kp = (0 - kp);
		ki = (0 - ki);
		kd = (0 - kd);
	}
	controllerDirection = Direction;
}
PID_SetSampleTime(int NewSampleTime)
{
	if (NewSampleTime > 0)
	{
		double ratio  = (double)NewSampleTime
		/ (double)SampleTime;
		ki *= ratio;
		kd /= ratio;
		SampleTime = (unsigned long)NewSampleTime;
	}
}
/* SetTunings(...)*************************************************************
 * This function allows the controller's dynamic performance to be adjusted. 
 * it's called automatically from the constructor, but tunings can also
 * be adjusted on the fly during normal operation
 ******************************************************************************/ 
PID_SetTunings(double Kp, double Ki, double Kd)
{
	if (Kp<0 || Ki<0 || Kd<0) return;
	
	dispKp = Kp; dispKi = Ki; dispKd = Kd;
	
	double SampleTimeInSec = ((double)SampleTime)/1000;
	kp = Kp;
	ki = Ki * SampleTimeInSec;
	kd = Kd / SampleTimeInSec;
	
	if(controllerDirection ==REVERSE)
	{
		kp = (0 - kp);
		ki = (0 - ki);
		kd = (0 - kd);
	}
}