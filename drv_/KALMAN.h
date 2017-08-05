#ifndef KALMAN_HEADER
#define KALMAN_HEADER
	/* Kalman filter variables */
    float Q_angle; // Process noise variance for the accelerometer
    float Q_bias; // Process noise variance for the gyro bias
    float R_measure; // Measurement noise variance - this is actually the variance of the measurement noise

    float angle; // The angle calculated by the Kalman filter - part of the 2x1 state vector
    float bias; // The gyro bias calculated by the Kalman filter - part of the 2x1 state vector
    float rate; // Unbiased rate calculated from the rate and the calculated bias - you have to call getAngle to update the rate

    float P[2][2]; // Error covariance matrix - This is a 2x2 matrix
	// The angle should be in degrees and the rate should be in degrees per second and the delta time in seconds
float getAngle(float newAngle, float newRate, float dt);
void Kalman_init() ;
#endif