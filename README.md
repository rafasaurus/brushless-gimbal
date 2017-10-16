# Brushless Gimbal

This software uses Kalman filter for data filtering of mpu6050 accelerometer
and gyroscope using i2c(TWI) interface for communication.
For controlling brushles DC motors software uses sinusoidal tables,
this can be generated on start of the program, or precalculated in microsoft excel,
or other software, see function.c file.
The code also uses PID controller for each axis.

This code is under [MIT OPEN SOURCE LICENSE](/LICENSE)
