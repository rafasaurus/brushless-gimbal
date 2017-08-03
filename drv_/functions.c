/*
 * functions.cpp
 *
 * Created: 7/12/2017 5:18:53 PM
 *  Author: rafa
 */ 

#include "functions.h"

void init_gpio()
{
	/*
	DDRB|=(1<<PB1);//digital 9 timer1   OC1A 
	DDRB|=(1<<PB2);//digital 10 timer1  OC1B
	DDRD|=(1<<PD3);//digital 3 timer2   OC2B
	DDRB|=(1<<PB3);//digital 11 timer2  OC2A
	DDRD|=(1<<PD5);//digital 6 timer0   OC0A
	DDRD|=(1<<PD6);//digital 5 timer0   OC0B
	//for debugging not pwm 7,8,2 pins used
	DDRD|=(1<<PD7);//enable 1 DIGITAL 7
	DDRB|=(1<<PB0);//enable 2 DIGITAL 8
	DDRD|=(1<<PD2);//enable 3 DIGITAL 2
	*/
	//DDRB|=(1<<PB5);//LED13
	//sbi(DDRB,4);//digital 10
	//sbi(DDRB,5);//digital 11
	//sbi(DDRB,6);//digital 12
	sbi(DDRH,3);//digital 6 OC4A
	sbi(DDRH,4);//digital 7 OC4B
	sbi(DDRH,5);//digital 8 OC4C
	//sbi(DDRH,6);//digital 9
}
void getSinTable(uint16_t sinTableSize,uint8_t *pwmSin,uint16_t sineScale)
{
	
	#ifdef SVPWM	
			double wave1;
			double wave2;
			double wave3;
			double sub1;
			double radWave;
			for (int i = 0; i < SINE_TABLE_SZ; i++)
			{
				double x = i * (2 * 3.14) / SINE_TABLE_SZ;
				wave1 = 255 * sin(x)+255;
				wave2 = 255 * sin(x-phase) + 255;
				wave3 = 255 * sin(x+phase) + 255;
				radWave = (min(wave1, wave2, wave3) + max(wave1, wave2, wave3)) / 2;
				sub1 = radWave - wave1;
				pwmSin[i]=(int)(-1*sub1)/2+128;
				uint16_t reg=pwmSin[i];
				print16(&reg);
				printf("\n");
			}		
	#else
		for (int i = 0; i < sinTableSize; i++)
		{
			double x = i * (2 * pi) / sinTableSize;
			pwmSin[i] = (sin(x) * sineScale) + sineScale;
			uint16_t reg=pwmSin[i];	
			print16(&reg);
			printf("\n");
		}
	#endif
}

double min (double a,double b, double c)
{
	if (a<b && a<c) return a;
	if (b<a && b<c) return b;
	if (c<a && c<b) return c;
}
double max (double a,double b, double c)
{
	if (a>b && a>c) return a;
	if (b>a && b>c) return b;
	if (c>a && c>b) return c;
}


