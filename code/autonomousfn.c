#pragma systemFile
#include "autonomous.h"
#include "robotParams.h"
#include "PIDControl.c"

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
	int gyroAvg = 0;

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
		int gyroV = getGyroDegrees(gyro);

		int difError = angleToEncoderUnits(-gyroV);

		bool hasReached = PIDControl(&controllerLeft, encoderTarget - leftEncoder, DRIVE_CLOSE_THRESHOLD, &motorSpeedLeft);
		bool isStraight = PIDControl(&controllerRight, difError, DRIVE_DIFF_THRESHOLD, &motorSpeedDiff);

		if ( hasReached && isStraight )
		{
			isComplete = true;
			break;
		}

		clipLR(motorSpeedLeft, motorSpeedDiff, &lastSpeedLeft, &lastSpeedRight, MAX_DRIVE_SPEED, MAX_DRIVE_ACCEL);
		LOG(5, lastSpeedLeft);
		LOG(6, lastSpeedRight);
		setMotorSpeed(leftWheels, convertToMotorSpeed(lastSpeedLeft));
		setMotorSpeed(rightWheels, convertToMotorSpeed(lastSpeedRight));
		sleep(LONG_INTERVAL);
	}

	setMotorSpeed(rightWheels, 0);
	setMotorSpeed(leftWheels, 0);

	resetGyro(gyro);

	sleep(25);
	return isComplete;
}

bool turnRobot(int angle) {
	PidObject controllerTurn;
	PidObject controllerDiff;
	bool isComplete = false;
	int offset = 0;

	for(int i = 0; i < 5; i++) {
		offset += getGyroDegrees(gyro);
		sleep(30);
	}

	offset /= 5;

	int encoderTarget = angleToEncoderUnits(-angle - offset);
	int lastSpeedLeft = 0;
	int lastSpeedRight = 0;

	resetMotorEncoder(leftWheels);
	resetMotorEncoder(rightWheels);
	resetGyro(gyro);

	PIDInit(&controllerTurn, 3, encoderTarget, /*COEFFICIENTS*/ 0.24, 0, 3.6, 0.95);
	PIDInit(&controllerDiff, 4, 0, /*COEFFICIENTS*/ 0.08, 0, 0.9, 0.6);

	while (!isCancelled())
	{
		// Retrieve Sensor Values
		int encoderLeft = round(getMotorEncoder(leftWheels));
		int encoderRight = round(getMotorEncoder(rightWheels));
		int gyroV = getGyroDegrees(gyro);
		int motorSpeedLeft;
		int motorSpeedDiff;

		int errorLeft = angleToEncoderUnits(-angle - gyroV);

		// Calculate Motor Speeds
		bool isCompleteTurn = PIDControl(&controllerTurn, errorLeft, TURN_CLOSE_THRESHOLD, &motorSpeedLeft);
		bool isCompleteAdjust = PIDControl(&controllerDiff, (encoderRight < 0) ? -(abs(encoderLeft) - abs(encoderRight)) : abs(encoderLeft) - abs(encoderRight), TURN_DIFF_THRESHOLD, &motorSpeedDiff);
		clipLR(motorSpeedLeft, motorSpeedDiff, &lastSpeedLeft, &lastSpeedRight, MAX_TURN_SPEED, MAX_DRIVE_ACCEL);


		// Check if complete
		if ( isCompleteTurn && isCompleteAdjust )
		{
			isComplete = true;
			break;
		}

		displayTextLine(3, "gyro = %d", gyroV);
		LOG(5, -lastSpeedLeft);
		LOG(6, lastSpeedRight);

		setMotorSpeed(leftWheels, convertToMotorSpeed(-lastSpeedLeft));
		setMotorSpeed(rightWheels, convertToMotorSpeed(lastSpeedRight));
		sleep(SHORT_INTERVAL);
	}

	setMotorSpeed(leftWheels, 0);
	setMotorSpeed(rightWheels, 0);

	resetGyro(gyro);

	sleep(25);

	return isComplete;
}

bool moveArm(int height) {
	PidObject controllerArm;
	bool isComplete;

	PIDInit(&controllerArm, 5, height, 0.5, 0, 0.2, 0.95);

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
	}

	setMotorSpeed(leftArm, 0);
	setMotorSpeed(rightArm, 0);
	sleep(25);
	return isComplete;
}
