// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
using namespace std;
double min(double a, double b, double c);
double max(double a, double b, double c);
int main()
{
	double wave1;
	double wave2;
	double wave3;
	double sub1;
	double sub2;
	double sub3;
	
	double radWave[720];
	for (int i = 0; i < 720; i++)
	{
		double x = i * (2 * 3.14) / 720;
		wave1 = 127 * sin(x)+127;
		wave2 = 127 * sin(x-80) + 127;
		wave3 = 127 * sin(x+80) + 127;
		radWave[i] = (min(wave1, wave2, wave3) + max(wave1, wave2, wave3)) / 2;
		sub1 = radWave[i] - wave1;
		sub2 = radWave[i] - wave2;
		sub3 = radWave[i] - wave3;
		//cout << wave2;
		cout << (-1*sub1)/2+128;
		printf("\n");
	}
    return 0;
}


double min(double a, double b, double c)
{
	if (a<b && a<c) return a;
	if (b<a && b<c) return b;
	if (c<a && c<b) return c;
}
double max(double a, double b, double c)
{
	if (a>b && a>c) return a;
	if (b>a && b>c) return b;
	if (c>a && c>b) return c;
}

