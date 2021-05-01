#pragma systemFile
#include "autonomous.h"
#include "robotParams.h"
#include "PIDControl.c"

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
		displayTextLine(2, "Do Not Move Robot");
		sleep(100);
	}
	displayTextLine(1, "Calibrated!");
	displayClearTextLine(2);
}

bool isCancelled()
{
	if(getTouchLEDValue(touch) == 1) {
		while(getTouchLEDValue(touch) != 0) {
			sleep(70);
		}

		setTouchLEDRGB(touch, 237, 28, 0);
		return true;
	}

	return false;
}

int angleToEncoderUnits(int angleDegrees) {
	int minAngle = angleDegrees;

	if (angleDegrees < -180) {
		minAngle = ((( angleDegrees - 180 ) % 360 ) + 180 );
	}
	else if ( angleDegrees > 180 ) {
		minAngle = ((( angleDegrees + 180 ) % 360 ) - 180 );
	}

	return round(ENC_UNITS_PER_DEGREE * minAngle);
}

bool driveRobot(int distanceInMM)
{
	PidObject controllerLeft;
	PidObject controllerRight;
	bool isComplete = false;
	int lastSpeedLeft = 0;
	int lastSpeedRight = 0;

	resetMotorEncoder(leftWheels);
	resetMotorEncoder(rightWheels);
	resetGyro(gyro);

	// Given how far we want to go in millimeters, find how far we want to go in encoder units
	int encoderTarget = round(distanceInMM * ENC_UNITS_PER_MM);

	PIDInit(&controllerLeft, 1, encoderTarget, /*COEFFICIENTS:*/ 0.04, 0, 0.6, 0.95);
	PIDInit(&controllerRight, 2, 0, /*COEFFICIENTS:*/ 0.04, 0.001, 0.5, 0.8);

	while (!isCancelled())
	{
		int motorSpeedLeft;
		int motorSpeedDiff;
		int leftEncoder = round(getMotorEncoder(leftWheels));
		int rightEncoder = round(getMotorEncoder(rightWheels));

		bool hasReached = PIDControl(&controllerLeft, encoderTarget - leftEncoder, THRESHOLD, &motorSpeedLeft);
		bool isStraight = PIDControl(&controllerRight, leftEncoder - rightEncoder, LEFT_RIGHT_THRESHOLD, &motorSpeedDiff);

		LOG(5, encoderTarget-leftEncoder);
		LOG(6, leftEncoder-rightEncoder);

		if ( (encoderTarget - leftEncoder) < CLOSE_THRESHOLD )
		{
			setMotorSpeed(leftWheels, 0);
			setMotorSpeed(rightWheels, 0);
			break;
		}

		clipLR(motorSpeedLeft, motorSpeedDiff, &lastSpeedLeft, &lastSpeedRight, MAX_DRIVE_SPEED, MAX_DRIVE_ACCEL);
		setMotorSpeed(leftWheels, convertToMotorSpeed(lastSpeedLeft));
		setMotorSpeed(rightWheels, convertToMotorSpeed(lastSpeedRight));
		sleep(LONG_INTERVAL);
	}

	setMotorSpeed(rightWheels, 0);
	setMotorSpeed(leftWheels, 0);

	sleep(25);
	return true;
}

bool turnRobot(int angle) {
	PidObject controllerTurn;
	PidObject controllerDiff;
	bool isComplete = false;
	int offset = 0;

	for(int i = 0; i < 5; i++) {
		offset += getGyroDegrees(gyro) / 5;
		sleep(30);
	}

	int encoderTarget = angleToEncoderUnits(-angle - offset);
	int lastSpeedLeft = 0;
	int lastSpeedRight = 0;

	resetMotorEncoder(leftWheels);
	resetMotorEncoder(rightWheels);
	resetGyro(gyro);

	PIDInit(&controllerTurn, 3, encoderTarget, /*COEFFICIENTS*/ 0.08, 0, 0.9, 0.95);
	PIDInit(&controllerDiff, 4, 0, /*COEFFICIENTS*/ 0.08, 0, 0.9, 0.6);

//	int gyroAvg = 0;

	while (!isCancelled())
	{
		// Retrieve Sensor Values
		int encoderLeft = round(getMotorEncoder(leftWheels));
		int encoderRight = round(getMotorEncoder(rightWheels));
		int motorSpeedLeft;
		int motorSpeedDiff;

	//	gyroAvg = round((TURN_AVG_KA * gyroAvg) + ((1 - TURN_AVG_KA) * getGyroDegreesFloat(gyro)));

	//	int pidErrorLeft = angleToEncoderUnits(-angle - gyroAvg);

		// Calculate Motor Speeds
		bool isCompleteTurn = PIDControl(&controllerTurn, encoderTarget + encoderLeft, THRESHOLD, &motorSpeedLeft);
		bool isCompleteAdjust = PIDControl(&controllerDiff, (encoderRight < 0) ? -(abs(encoderLeft) - abs(encoderRight)) : abs(encoderLeft) - abs(encoderRight), THRESHOLD, &motorSpeedDiff);
		clipLR(motorSpeedLeft, motorSpeedDiff, &lastSpeedLeft, &lastSpeedRight, MAX_TURN_SPEED, MAX_DRIVE_ACCEL);


		// Check if complete
		if ( abs(encoderTarget + encoderLeft) < CLOSE_THRESHOLD )
		{
			break;
		}

		LOG(5, -lastSpeedLeft);
		LOG(6, lastSpeedRight);

		setMotorSpeed(leftWheels, convertToMotorSpeed(-lastSpeedLeft));
		setMotorSpeed(rightWheels, convertToMotorSpeed(lastSpeedRight));
		sleep(SHORT_INTERVAL);
	}

	setMotorTarget(leftWheels, -encoderTarget, 60);
	setMotorTarget(rightWheels, encoderTarget, 60);

	WAIT_FOR_MOTOR(rightWheels);
	WAIT_FOR_MOTOR(leftWheels);

	resetGyro(gyro);

	sleep(25);

	return true;
}

bool moveArm(int height) {
	PidObject controllerArm;
	bool isComplete;

	setMotorTarget(leftArm, height, 30);
	setMotorTarget(rightArm, height, 30);

	WAIT_FOR_MOTOR(leftArm);
	WAIT_FOR_MOTOR(rightArm);
/*	PIDInit(&controllerArm, 8, height, 0.3, 0, 0.5, 0.95);

	int lastSpeed = 0;

	while(!isCancelled()) {
		int motorSpeed;

		isComplete = PIDControl(&controllerArm, height - getMotorEncoder(leftArm), THRESHOLD_ARM, &motorSpeed);
		motorSpeed = clip(motorSpeed, lastSpeed, MAX_ARM_SPEED, MAX_ARM_ACCEL);
		lastSpeed = motorSpeed;

		if(isComplete) {
			break;
		}

		LOG(5, motorSpeed);
		setMotorSpeed(leftArm, convertToMotorSpeed(motorSpeed));
		setMotorSpeed(rightArm, convertToMotorSpeed(motorSpeed));
		sleep(SHORT_INTERVAL);
	}*/

	sleep(25);
	return true;
}
