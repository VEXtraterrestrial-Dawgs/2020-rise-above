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

	// Datalog
	datalogDataGroupStart();
	datalogAddValue( 0, pid->controllerIndex );
	datalogAddValue( 1, error );
	datalogAddValue( 2, round(pid->integral*1000) );
	datalogAddValue( 3, round(derivative*1000) );
	datalogDataGroupEnd();

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

	controllerLeft.Kp = 1;
	controllerLeft.Ki = 0;
	controllerLeft.Kd = 0;
	controllerLeft.Ka = 0;
	controllerLeft.controllerIndex = 1;

	controllerRight.Kp = 0;
	controllerRight.Ki = 0;
	controllerRight.Kd = 0;
	controllerRight.Ka = 0;
	controllerRight.controllerIndex = 2;

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
	PidObject controllerLeftTurn;
	PidObject controllerRightTurn;
	float angleRadians = degreesToRadians(angle);
	float distanceMM = (DRIVETRAIN_WIDTH / 2) * angleRadians;
	float distanceEncoders = (distanceMM / (WHEEL_CIRCUMFERENCE * DRIVE_GEAR_RATIO))*ENCODER_UNITS_PER_ROTATION;
	bool isComplete = false;

	resetMotorEncoder(leftWheels);
	resetMotorEncoder(rightWheels);
	resetGyro(gyro);

	controllerLeftTurn.Kp = 1;
	controllerLeftTurn.Ki = 0;
	controllerLeftTurn.Kd = 0;
	controllerLeftTurn.Ka = 0;

	controllerRightTurn.Kp = 1;
	controllerRightTurn.Ki = 0;
	controllerRightTurn.Kd = 0;
	controllerRightTurn.Ka = 0;

	while (!isCancelled())
	{
		// Retrieve Sensor Values
		int encoderLeft = round(getMotorEncoder(leftWheels));
		int encoderRight = round(getMotorEncoder(rightWheels));
		float gyroDegrees = getGyroDegreesFloat(gyro);

		// Correct Sensor Values
		float heading = gyroDegrees;

		// Calculate Motor Speeds
		bool isCompleteLeft;
		bool isCompleteRight;
		int motorSpeedLeft;
		int motorSpeedRight;

		isCompleteLeft = PIDControl(&controllerLeftTurn, (distanceEncoders * -1), encoderLeft, THRESHOLD, &motorSpeedLeft);
		setMotorSpeed(leftWheels, motorSpeedLeft);
		isCompleteRight = PIDControl(&controllerRightTurn, (encoderLeft * -1), encoderRight, THRESHOLD, &motorSpeedRight);
		setMotorSpeed(rightWheels, motorSpeedRight);

		/*
		isCompleteLeft = PIDControl(&controllerLeftTurn, angle, heading, TURN_THRESHOLD, &motorSpeedLeft);
		setMotorSpeed(leftWheels, motorSpeedLeft);
		isCompleteRight = PIDControl(&controllerRightTurn, (encoderLeft * -1), encoderRight, THRESHOLD, &motorSpeedRight);
		setMotorSpeed(rightWheels, motorSpeedRight);
		*/

		// Check if complete
		if (isCompleteRight && isCompleteLeft)
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
	PidObject controllerHDrive;
	int encoderTarget = (distance / (WHEEL_CIRCUMFERENCE * H_DRIVE_GEAR_RATIO))*ENCODER_UNITS_PER_ROTATION;
	bool isComplete;
	int motorSpeed;

	resetMotorEncoder(hDrive);

	controllerHDrive.Kp = 1;
	controllerHDrive.Ki = 0;
	controllerHDrive.Kd = 0;
	controllerHDrive.Ka = 0;

	while(!isCancelled()) {
		isComplete = PIDControl(&controllerHDrive, encoderTarget, getMotorEncoder(hDrive), THRESHOLD, &motorSpeed);
		setMotorSpeed(hDrive, motorSpeed);

		if(isComplete) {
			break;
		}
		sleep(LONG_INTERVAL);
	}

	setMotorSpeed(hDrive, 0);
	return isComplete;
}

bool moveTopArm(int height) {
	PidObject controllerTopArm;
	bool isComplete;
	int motorSpeed;

	resetMotorEncoder(armHigh);

	controllerTopArm.Kp = 1;
	controllerTopArm.Ki = 0;
	controllerTopArm.Kd = 0;
	controllerTopArm.Ka = 0;

	while(!isCancelled()) {
		isComplete = PIDControl(&controllerTopArm, height, getMotorEncoder(armHigh), THRESHOLD_ARM, &motorSpeed);
		setMotorSpeed(armHigh, motorSpeed);

		if(isComplete) {
			break;
		}

		sleep(SHORT_INTERVAL);
	}

	setMotorSpeed(armHigh, 0);
	return isComplete;
}

bool moveBottomArm(int height) {
	PidObject controllerBottomArm;
	bool isComplete;
	int motorSpeed;

	resetMotorEncoder(armHigh);

	controllerBottomArm.Kp = 1;
	controllerBottomArm.Ki = 0;
	controllerBottomArm.Kd = 0;
	controllerBottomArm.Ka = 0;

	while(!isCancelled()) {
		isComplete = PIDControl(&controllerBottomArm, height, getMotorEncoder(armLow), THRESHOLD_ARM, &motorSpeed);
		setMotorSpeed(armLow, motorSpeed);

		if(isComplete) {
			break;
		}

		sleep(SHORT_INTERVAL);
		}

	setMotorSpeed(armLow, 0);
	return isComplete;
}
