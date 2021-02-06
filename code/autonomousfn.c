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

		if (hasReached && isStraight)
		{
			isComplete = true;
			break;
		}

		clipLR(motorSpeedLeft, motorSpeedDiff, &lastSpeedLeft, &lastSpeedRight, MAX_DRIVE_SPEED, MAX_DRIVE_ACCEL);
		setMotorSpeed(leftWheels, convertToMotorSpeed(lastSpeedLeft));
		setMotorSpeed(rightWheels, convertToMotorSpeed(lastSpeedRight));
		sleep(LONG_INTERVAL);
	}

	setMotorSpeed(leftWheels, 0);
	setMotorSpeed(rightWheels, 0);
	return isComplete;
}

bool turnRobot(int angle) {
	PidObject controllerTurn;
	PidObject controllerDiff;
	bool isComplete = false;
	int encoderTarget = angleToEncoderUnits(-angle);
	int lastSpeedLeft = 0;
	int lastSpeedRight = 0;

	resetMotorEncoder(leftWheels);
	resetMotorEncoder(rightWheels);
	resetGyro(gyro);

	PIDInit(&controllerTurn, 3, encoderTarget, /*COEFFICIENTS*/ 0.08, 0, 0.9, 0.95);
	PIDInit(&controllerDiff, 4, 0, /*COEFFICIENTS*/ 0.08, 0, 0.9, 0.6);

	int gyroAvg = 0;

	while (!isCancelled())
	{
		// Retrieve Sensor Values
		int encoderLeft = round(getMotorEncoder(leftWheels));
		int encoderRight = round(getMotorEncoder(rightWheels));
		int motorSpeedLeft;
		int motorSpeedDiff;

		gyroAvg = round((TURN_AVG_KA * gyroAvg) + ((1 - TURN_AVG_KA) * getGyroDegreesFloat(gyro)));

		int pidErrorLeft = angleToEncoderUnits(-angle - gyroAvg);

		// Calculate Motor Speeds
		bool isCompleteTurn = PIDControl(&controllerTurn, pidErrorLeft, THRESHOLD, &motorSpeedLeft);
		bool isCompleteAdjust = PIDControl(&controllerDiff, (encoderRight < 0) ? -(abs(encoderLeft) - abs(encoderRight)) : abs(encoderLeft) - abs(encoderRight), THRESHOLD, &motorSpeedDiff);
		clipLR(motorSpeedLeft, motorSpeedDiff, &lastSpeedLeft, &lastSpeedRight, MAX_TURN_SPEED, MAX_DRIVE_ACCEL);


		// Check if complete
		if (isCompleteTurn && isCompleteAdjust)
		{
			isComplete = true;
			break;
		}

		LOG(5, -lastSpeedLeft);
		LOG(6, lastSpeedRight);

		setMotorSpeed(leftWheels, convertToMotorSpeed(-lastSpeedLeft));
		setMotorSpeed(rightWheels, convertToMotorSpeed(lastSpeedRight));
		sleep(SHORT_INTERVAL);
	}

	setMotorSpeed(leftWheels, 0);
	setMotorSpeed(rightWheels, 0);

	return isComplete;
}

bool moveHDrive(int distance) {
	PidObject controllerHDrive;
	bool isComplete;
	int encoderTarget = (distance * ENCODER_UNITS_PER_ROTATION / (WHEEL_CIRCUMFERENCE * H_DRIVE_GEAR_RATIO));

	resetMotorEncoder(hDrive);

	PIDInit(&controllerHDrive, 5, encoderTarget, 0.5, 0, 0.4, 0.95);

	int lastSpeed = 0;

	while(!isCancelled()) {
		int motorSpeed;

		isComplete = PIDControl(&controllerHDrive, encoderTarget - getMotorEncoder(hDrive), THRESHOLD, &motorSpeed);
		motorSpeed = clip(motorSpeed, lastSpeed, MAX_H_DRIVE_SPEED, MAX_DRIVE_ACCEL);
		lastSpeed = motorSpeed;

		if(isComplete) {
			break;
		}

		LOG(5, motorSpeed);
		LOG(1, encoderTarget - getMotorEncoder(hDrive));

		setMotorSpeed(hDrive, convertToMotorSpeed(motorSpeed));
		sleep(SHORT_INTERVAL);
	}

	setMotorSpeed(hDrive, 0);
	return isComplete;
}

bool moveArm(int height) {
	PidObject controllerArm;
	bool isComplete;

	PIDInit(&controllerArm, 8, height, 0.3, 0, 0.5, 0.95);

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
	return isComplete;
}
