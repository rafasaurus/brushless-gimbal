# Brushless Gimbal
This software drives 3 axis brushless gimbal using DRV8313 drivers(by Texas Instruments),
and Atmel AVR Atmega2560 microcontroller.
This software uses Kalman filter for data filtering of mpu6050 accelerometer
and gyroscope using i2c(TWI) interface for communication.
For controlling brushles DC motors. Software uses sinusoidal tables,
this can be generated at the beginning of the program, or precalculated in
microsoft excel or other software. See function.c file.
The code also uses PID controller for regulation of each axis speed.

This project is made by using Atmel Studio IDE, you can use it for your convenience.

This code is under [MIT OPEN SOURCE LICENSE](/LICENSE)
