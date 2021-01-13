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
	if(getTouchLEDValue(touch) == 1) {
		while(getTouchLEDValue(touch) != 0) {
			sleep(70);
		}

		setTouchLEDRGB(touch, 237, 28, 0);
		return true;
	}

	return false;
}

void waitForLED() {
	while(getTouchLEDValue(touch) != 1)
	{
		sleep(70);
	}

	while(getTouchLEDValue(touch) != 0)
	{
		sleep(70);
	}
}

int convertToMotorSpeed(int proposed) {
	if(proposed == 0) {
		return 0;
	}

	if(proposed < 0) {
		return round( (float)proposed * MOTOR_RANGE / 100 ) - MOTOR_LOW;
	}

	return round( (float)proposed * MOTOR_RANGE / 100 ) + MOTOR_LOW;

}

void PIDInit(PidObject* pid, int controllerIndex, int initialError, float Kp, float Ki, float Kd, float Ka) {
	pid->controllerIndex = controllerIndex;
	pid->lastError = initialError;
	pid->lastTime = nPgmTime;
	pid->Kp = Kp;
	pid->Ki = Ki;
	pid->Kd = Kd;
	pid->Ka = Ka;
	pid->integral = 0;
}

bool PIDControl(PidObject* pid, int error, int threshold, int* power) {
	int now = nPgmTime;
	int elapsed = now - pid->lastTime;
	float derivative;

	if(elapsed == 0) {
		derivative = 0;
	}
	else {
		derivative = ( (float)error - pid->lastError ) / elapsed;
	}

	pid->integral *= pid->Ka;
	pid->integral += (error + pid->lastError) * elapsed;

	pid->lastError = error;
	pid->lastTime = now;

	// Datalog
	LOG_WITH_TIME( 0, pid->controllerIndex );
	LOG( 1, error );
	LOG( 2, round(pid->integral*1000) );
	LOG( 3, round(derivative*1000) );
	LOG( 4, getGyroDegrees(gyro) );

	// Returns a motor speed
	*power = round( (error * pid->Kp) + (pid->integral * pid->Ki) + (derivative * pid->Kd) );

	return (abs(error) < threshold);
}

int clip(int proposedSpeed, int lastSpeed, int maxSpeed, int maxAcceleration)
{
	if ( proposedSpeed > maxSpeed ) {
		proposedSpeed = maxSpeed;
	}
	else if ( proposedSpeed < -maxSpeed ) {
		proposedSpeed = -maxSpeed;
	}

	if (( lastSpeed >= 0 ) &&
		  ( proposedSpeed > ( lastSpeed + maxAcceleration ) )) {
		proposedSpeed = lastSpeed + maxAcceleration;
	}
	if (( lastSpeed <= 0 ) &&
		  ( proposedSpeed < ( lastSpeed - maxAcceleration ) )) {
		proposedSpeed = lastSpeed - maxAcceleration;
	}

	return proposedSpeed;
}

void clipLR(int proposedLeft, int proposedDifference, int* speedLeft, int* speedRight, int maxSpeed, int maxAcceleration)
{
	int clipLeft;
	int clipRight;
	int newDiff;

	if ( proposedDifference > MAX_DRIVE_DIFFERENCE ) {
		newDiff = MAX_DRIVE_DIFFERENCE;
	}
	else if ( proposedDifference < -MAX_DRIVE_DIFFERENCE ) {
		newDiff = -MAX_DRIVE_DIFFERENCE;
	}
	else {
		newDiff = proposedDifference;
	}

	clipLeft = clip(proposedLeft, *speedLeft, maxSpeed, maxAcceleration);
	clipRight = clip(proposedLeft + newDiff, *speedRight, maxSpeed, maxAcceleration);

	int maxClipDiff = proposedLeft - clipLeft;

	if (abs( (proposedLeft + newDiff) - clipRight) > abs(maxClipDiff) ) {
		maxClipDiff = (proposedLeft + newDiff) - clipRight;
	}

	*speedLeft = proposedLeft - maxClipDiff;
	*speedRight = (proposedLeft + newDiff) - maxClipDiff;
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
	PidObject controllerRight;
	bool isComplete = false;

	resetMotorEncoder(leftWheels);
	resetMotorEncoder(rightWheels);
	resetGyro(gyro);

	// Given how far we want to go in millimeters, find how far we want to go in encoder units
	int encoderTarget = round(distanceInMM * ENC_UNITS_PER_MM);

	PIDInit(&controllerRight, 2, 0, /*COEFFICIENTS:*/ 0.07, 0.01, 1, 0.4);

	setMotorTarget(leftWheels, encoderTarget, 60);
	setMotorSpeed(rightWheels, 60);

	while (!isCancelled())
	{
		int motorSpeedLeft;
		int motorSpeedDiff;
		int leftEncoder = round(getMotorEncoder(leftWheels));
		int rightEncoder = round(getMotorEncoder(rightWheels));
		int leftSpeed = getMotorSpeed(leftWheels);

		PIDControl(&controllerRight, leftEncoder - rightEncoder, LEFT_RIGHT_THRESHOLD, &motorSpeedDiff);

		LOG(5, encoderTarget-leftEncoder);
		LOG(6, leftEncoder-rightEncoder);

		if ( (encoderTarget - leftEncoder) < CLOSE_THRESHOLD )
		{
			break;
		}

		if(motorSpeedDiff + leftSpeed > 100) {
			motorSpeedDiff = 100;
		}
		else {
			motorSpeedDiff += leftSpeed;
		}

		setMotorSpeed(rightWheels, convertToMotorSpeed(motorSpeedDiff));
		sleep(LONG_INTERVAL);
	}

	setMotorTarget(rightWheels, encoderTarget, 60);
	waitUntilMotorStop(leftWheels);
	waitUntilMotorStop(rightWheels);
	return true;
}

bool turnRobot(int angle) {
	PidObject controllerDiff;
	bool isComplete = false;
	int encoderTarget = angleToEncoderUnits(-angle);

	displayTextLine(4, "encoderTarget: %d", encoderTarget);

	resetMotorEncoder(leftWheels);
	resetMotorEncoder(rightWheels);
	resetGyro(gyro);

	PIDInit(&controllerDiff, 4, 0, /*COEFFICIENTS*/ 20, 0.1, 15, 0.8);

	setMotorTarget(leftWheels, -encoderTarget, 70);
	setMotorSpeed(rightWheels, 70);

	while (!isCancelled())
	{
		// Retrieve Sensor Values
		int encoderLeft = round(getMotorEncoder(leftWheels));
		int encoderRight = round(getMotorEncoder(rightWheels));
		int motorSpeedRight;
		int motorSpeedDiff;
		int leftSpeed = -getMotorSpeed(leftWheels);

		// Calculate Motor Speeds
		PIDControl(&controllerDiff, abs(encoderLeft) - abs(encoderRight), THRESHOLD, &motorSpeedDiff);

		// Check if complete
		if ( abs(-encoderTarget - encoderLeft) < CLOSE_THRESHOLD )
		{
			break;
		}

		if(leftSpeed < 0) {
			if((leftSpeed - motorSpeedDiff) < -100) {
				motorSpeedRight = -100;
			}
			else {
				motorSpeedRight = leftSpeed - motorSpeedDiff;
			}
		}
		else {
			if((motorSpeedDiff + leftSpeed) > 100) {
				motorSpeedRight = 100;
			}
			else {
				motorSpeedRight = motorSpeedDiff + leftSpeed;
			}
		}

		// Set Motor Speeds
		setMotorSpeed(rightWheels, convertToMotorSpeed(motorSpeedRight));
		sleep(SHORT_INTERVAL);
	}

	setMotorTarget(rightWheels, encoderTarget, 70);
	waitUntilMotorStop(leftWheels);
	waitUntilMotorStop(rightWheels);
	return true;
}

bool moveHDrive(int distance) {
	PidObject controllerHDrive;
	bool isComplete;
	int encoderTarget = (distance * ENCODER_UNITS_PER_ROTATION / (WHEEL_CIRCUMFERENCE * H_DRIVE_GEAR_RATIO));

	resetMotorEncoder(hDrive);

	PIDInit(&controllerHDrive, 5, encoderTarget, 0.3, 0, 0.8, 0.95);

	int lastSpeed = 0;

	while(!isCancelled()) {
		int motorSpeed;

		isComplete = PIDControl(&controllerHDrive, encoderTarget - getMotorEncoder(hDrive), THRESHOLD, &motorSpeed);
		motorSpeed = clip(motorSpeed, lastSpeed, MAX_DRIVE_SPEED, MAX_DRIVE_ACCEL);
		lastSpeed = motorSpeed;

		if(isComplete) {
			break;
		}

		LOG(5, motorSpeed);

		setMotorSpeed(hDrive, convertToMotorSpeed(motorSpeed));
		sleep(SHORT_INTERVAL);
	}

	setMotorSpeed(hDrive, 0);
	return isComplete;
}

bool moveArm(tMotor arm, int controlIndex, int height) {
	PidObject controllerArm;
	bool isComplete;

	PIDInit(&controllerArm, controlIndex, height, 0.5, 0, 10, 0.95);

	int lastSpeed = 0;

	while(!isCancelled()) {
		int motorSpeed;

		isComplete = PIDControl(&controllerArm, height - getMotorEncoder(arm), THRESHOLD_ARM, &motorSpeed);
		motorSpeed = clip(motorSpeed, lastSpeed, MAX_ARM_SPEED, MAX_ARM_ACCEL);
		lastSpeed = motorSpeed;

		if(isComplete) {
			break;
		}

		LOG(5, motorSpeed);
		setMotorSpeed(arm, convertToMotorSpeed(motorSpeed));
		sleep(SHORT_INTERVAL);
	}

	setMotorSpeed(arm, 0);
	return isComplete;
}

bool moveTopArm(int height) {
	return moveArm(armHigh, 6, height);
}

bool moveLowerArm(int height) {
	return moveArm(armLow, 7, height);
}
