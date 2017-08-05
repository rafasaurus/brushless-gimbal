#include "MPU6050.h"

void mpu6050_calibrate_gyro(int32_t *x,int32_t *y, int32_t *z)
{
	printf("\n Calibrating Gyro...\n");
	for (int i=0;i<calibration_counter;i++)
	{
		(*x)+=mpu6050_read_gyroX();
		(*y)+=mpu6050_read_gyroY();
		(*z)+=mpu6050_read_gyroZ();
		_delay_ms(caliberation_wait_delay);
	}
		if(*x<0)
		(*x)=abs(*x)/calibration_counter;
		else
		(*x)=(*x)/calibration_counter;
		
		if(*y<0)
		(*y)=abs(*y)/calibration_counter;
		else
		(*y)=(*y)/calibration_counter;
		
		if(*z<0)
		(*z)=abs(*z)/calibration_counter;
		else
		(*z)=(*z)/calibration_counter;
		//------------do not delete----------
	printf("gyro offsets are");
	print16((int16_t *)x);
	printf("  ");
	print16((int16_t *)y);
	printf("  ");
	print16ln((int16_t *)z);
	_delay_ms(1000);
	//-----------------------------------
				
	
}

void mpu6050_calibrate_accel(int32_t *x,int32_t *y, int32_t *z)
{
	
	printf("Calibrating Accelerometer...\n");
	for(int i=0;i<calibration_counter;i++){
		(*x)+=mpu6050_read_accelX();
		(*y)+=mpu6050_read_accelY();
		(*z)+=mpu6050_read_accelZ();
		_delay_ms(caliberation_wait_delay);
	}
	if(*x<0)
	(*x)=abs(*x)/calibration_counter;
	else
	(*x)=(*x)/calibration_counter;
	
	if(*y<0)
	(*y)=abs(*y)/calibration_counter;
	else
	(*y)=(*y)/calibration_counter;
	
	if(*z<0)
	(*z)=abs(*z)/calibration_counter;
	else
	(*z)=(*z)/calibration_counter;
	
	/*
	//------------do not delete----------
	printf("accel offsets are");
	print16(x);
	printf("  ");
	print16(y);
	printf("  ");
	print16ln(z);
	_delay_ms(1000);
	//-----------------------------------
	*/
}

uint16_t mpu6050_read_gyroX()//axis x=0,y=1,z=2
{
	uint16_t axis_value=mpu6050_readByte(MPU6050_RA_GYRO_XOUT_H);
	axis_value=(axis_value<<8) | mpu6050_readByte(MPU6050_RA_GYRO_XOUT_L);
	return axis_value;
}
uint16_t mpu6050_read_gyroY()//axis x=0,y=1,z=2
{
	uint16_t axis_value=mpu6050_readByte(MPU6050_RA_GYRO_YOUT_H);
	axis_value=(axis_value<<8) | mpu6050_readByte(MPU6050_RA_GYRO_YOUT_L);
	return axis_value;
}
uint16_t mpu6050_read_gyroZ()//axis x=0,y=1,z=2
{
	uint16_t axis_value=mpu6050_readByte(MPU6050_RA_GYRO_ZOUT_H);
	axis_value=(axis_value<<8) | mpu6050_readByte(MPU6050_RA_GYRO_ZOUT_L);
	return axis_value;
}
uint16_t mpu6050_read_accelX()
{
	uint16_t axis_value=mpu6050_readByte(MPU6050_RA_ACCEL_XOUT_H);
	axis_value=(axis_value<<8) | mpu6050_readByte(MPU6050_RA_ACCEL_XOUT_L);
	return axis_value;
}
uint16_t mpu6050_read_accelY()
{
	uint16_t axis_value=mpu6050_readByte(MPU6050_RA_ACCEL_YOUT_H);
	axis_value=(axis_value<<8) | mpu6050_readByte(MPU6050_RA_ACCEL_YOUT_L);
	return axis_value;
}
uint16_t mpu6050_read_accelZ()
{
	uint16_t axis_value=mpu6050_readByte(MPU6050_RA_ACCEL_ZOUT_H);
	axis_value=(axis_value<<8) | mpu6050_readByte(MPU6050_RA_ACCEL_ZOUT_L);
	return axis_value;
}
uint8_t mpu6050_readByte(uint8_t byteToRead)//readbyte
{
	i2c_start(MPU6050_WRITE);
	i2c_write(byteToRead);
	i2c_stop();
	
	i2c_start(MPU6050_READ);
	uint8_t byteToReturn=i2c_readNak();
	i2c_stop();
	return byteToReturn;
}
void mpu6050_writeByte(uint8_t wereToWrite,uint8_t byteToWrite)//writebyte
{
	i2c_start(MPU6050_WRITE);
	i2c_write(wereToWrite);
	i2c_write(byteToWrite);
	i2c_stop();
}
int8_t mpu6050_readBytes(uint8_t regAddr, uint8_t length, uint8_t *data) {
	uint8_t i = 0;
	int8_t count = 0;
	if(length > 0) {
		//request register
		i2c_start(MPU6050_WRITE);
		i2c_write(regAddr);
		_delay_us(10);
		//read data
		i2c_start(MPU6050_READ);
		for(i=0; i<length; i++) {
			count++;
			if(i==length-1)
			data[i] = i2c_readNak();
			else
			data[i] = i2c_readAck();
		}
		i2c_stop();
	}
	return count;
}
void mpu6050_getRawData(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz) {
	uint8_t buffer[14];
	mpu6050_readBytes(MPU6050_RA_ACCEL_XOUT_H, 14,  buffer);
	*ax = (((int16_t)buffer[0]) << 8) | buffer[1];
	*ay = (((int16_t)buffer[2]) << 8) | buffer[3];
	*az = (((int16_t)buffer[4]) << 8) | buffer[5];
	*gx = (((int16_t)buffer[8]) << 8) | buffer[9];
	*gy = (((int16_t)buffer[10]) << 8) | buffer[11];
	*gz = (((int16_t)buffer[12]) << 8) | buffer[13];
}