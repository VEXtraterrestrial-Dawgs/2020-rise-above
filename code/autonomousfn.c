#pragma systemFile
#include "autonomous.h"

void calibrateGyro()
{
	startGyroCalibration(gyro, gyroCalibrateSamples512);

	// delay so calibrate flag can be set internally to the gyro
	sleep(100);

	eraseDisplay();

	// wait for calibration to finish or 2 seconds, whichever is longer
	for (int i = 0; getGyroCalibrationFlag(gyro) || (i < 20); i++)
	{
		displayTextLine(1, "Calibrating... %02d", i);
		sleep(100);
	}
	displayTextLine(1, "Calibrated!");
}

bool isCancelled()
{
	// TODO: Read the touch LED and return true if it's been touched.
	return false;
}

bool PIDControl(PidObject* pid, int target, int current, int threshold, int* power) {
	int error = target - current;
	int now = nPgmTime;
	int elapsed = now - pid->lastTime;
	float derivative = ( error - pid->lastError ) / elapsed;

	pid->integral *= pid->Ka;
	pid->integral += (error + pid->lastError) * elapsed;

	pid->lastError = error;
	pid->lastTime = now;

	// Returns a motor speed
	*power = round( (error * pid->Kp) + (pid->integral * pid->Ki) + (derivative * pid->Kd) );

	return (error < threshold);
}

bool driveRobot(int distanceInMM)
{
	PidObject controllerLeft;
	PidObject controllerRight;
	bool isComplete = false;

	resetMotorEncoder(leftWheels);
	resetMotorEncoder(rightWheels);
	resetGyro(gyro);

	// Initialize PID controller coefficients to placeholder values
	controllerLeft.Kp = 1;
	controllerLeft.Ki = 0;
	controllerLeft.Kd = 0;
	controllerLeft.Ka = 0;

	controllerRight.Kp = 1;
	controllerRight.Ki = 0;
	controllerRight.Kd = 0;
	controllerRight.Ka = 0;

	// Given how far we want to go in millimeters, find how far we want to go in encoder units
	int encoderTarget = (distanceInMM / (WHEEL_CIRCUMFERENCE * DRIVE_GEAR_RATIO)) * ENCODER_UNITS_PER_ROTATION;

	while (!isCancelled())
	{
		int motorSpeedLeft;
		int motorSpeedRight;
		int leftEncoder = round(getMotorEncoder(leftWheels));
		int rightEncoder = round(getMotorEncoder(rightWheels));
		bool isLeftComplete;
		bool isRightComplete;

		isLeftComplete = PIDControl(&controllerLeft, encoderTarget, leftEncoder, THRESHOLD, &motorSpeedLeft);
		setMotorSpeed(leftWheels, motorSpeedLeft);
		isRightComplete = PIDControl(&controllerRight, leftEncoder, rightEncoder, THRESHOLD, &motorSpeedRight);
		setMotorSpeed(rightWheels, motorSpeedRight);

		if (isLeftComplete && isRightComplete)
		{
			isComplete = true;
			break;
		}
		sleep(75);
	}

	setMotorSpeed(leftWheels, 0);
	setMotorSpeed(rightWheels, 0);
	return isComplete;
}

bool turnRobot(int angle) {
	float angleRadians = degreesToRadians(angle);
	float distanceMM = (DRIVETRAIN_WIDTH / 2) * angleRadians;
	float distanceEncoders = (distanceMM / (WHEEL_CIRCUMFERENCE * DRIVE_GEAR_RATIO))*ENCODER_UNITS_PER_ROTATION;
	bool isComplete = false;

	resetMotorEncoder(leftWheels);
	resetMotorEncoder(rightWheels);
	resetGyro(gyro);

	while (!isCancelled())
	{
		// Retrieve Sensor Values
		int encoderLeft = round(getMotorEncoder(leftWheels));
		int encoderRight = round(getMotorEncoder(rightWheels));
		float gyroDegrees = getGyroDegreesFloat(gyro);

		// Correct Sensor Values
		float heading = gyroDegrees;

		// Calculate Motor Speeds

		// Check if complete
		if (false)
		{
			isComplete = true;
			break;
		}

		// Set Motor Speeds

		sleep(SHORT_INTERVAL);
	}

	setMotorSpeed(leftWheels, 0);
	setMotorSpeed(rightWheels, 0);
	return isComplete;
}

bool moveHDrive(int distance) {
	return true;
}

bool moveTopArm(int height) {
	return true;
}

bool moveBottomArm(int height) {
	return true;
}

bool moveBothArms(int height) {
	return true;
}
