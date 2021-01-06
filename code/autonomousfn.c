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
	int minAngle = ((( angleDegrees + 180 ) % 360 ) - 180 );
	return round(( DRIVETRAIN_WIDTH * 0.5 * degreesToRadians(minAngle) * ENCODER_UNITS_PER_ROTATION ) /
	  					( WHEEL_CIRCUMFERENCE * DRIVE_GEAR_RATIO ));
}

bool driveRobot(int distanceInMM)
{
	PidObject controllerLeft;
	PidObject controllerRight;
	bool isComplete = false;

	resetMotorEncoder(leftWheels);
	resetMotorEncoder(rightWheels);
	resetGyro(gyro);

	// Given how far we want to go in millimeters, find how far we want to go in encoder units
	int encoderTarget = (distanceInMM * ENCODER_UNITS_PER_ROTATION) / (WHEEL_CIRCUMFERENCE * DRIVE_GEAR_RATIO);
	int lastSpeedLeft = 0;
	int lastSpeedRight = 0;

	PIDInit(&controllerLeft, 1, encoderTarget, 0.3, 0, 0.5, 0.95);
	PIDInit(&controllerRight, 2, 0, 0.3, 0.005, 0.4, 0.95);

	while (!isCancelled())
	{
		int motorSpeedLeft;
		int motorSpeedDiff;
		int leftEncoder = round(getMotorEncoder(leftWheels));
		int rightEncoder = round(getMotorEncoder(rightWheels));
		bool hasReached;
		bool isStraight;

		hasReached = PIDControl(&controllerLeft, encoderTarget - leftEncoder, THRESHOLD, &motorSpeedLeft);
		isStraight = PIDControl(&controllerRight, leftEncoder - rightEncoder, THRESHOLD, &motorSpeedDiff);

		if (hasReached && isStraight)
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

	setMotorSpeed(leftWheels, 0);
	setMotorSpeed(rightWheels, 0);
	return isComplete;
}

bool turnRobot(int angle) {
	PidObject controllerTurn;
	PidObject controllerDiff;
	bool isComplete = false;
	int pidErrorLeft = angleToEncoderUnits(-angle);
	int lastSpeedLeft = 0;
	int lastSpeedRight = 0;

	resetMotorEncoder(leftWheels);
	resetMotorEncoder(rightWheels);
	resetGyro(gyro);

	PIDInit(&controllerTurn, 3, pidErrorLeft, 5, 0, 20, 0.95);
	PIDInit(&controllerDiff, 4, 0, 3, 0.05, 80, 1);

	while (!isCancelled())
	{
		// Retrieve Sensor Values
		int encoderLeft = -round(getMotorEncoder(leftWheels));
		int encoderRight = round(getMotorEncoder(rightWheels));
		bool isCompleteTurn;
		bool isCompleteAdjust;
		int motorSpeedLeft;
		int motorSpeedDiff;

		// Correct Sensor Values
	  pidErrorLeft = angleToEncoderUnits(-angle - getGyroDegrees(gyro));

		// Calculate Motor Speeds
		isCompleteTurn = PIDControl(&controllerTurn, pidErrorLeft, THRESHOLD, &motorSpeedLeft);
		isCompleteAdjust = PIDControl(&controllerDiff, encoderLeft - encoderRight, THRESHOLD, &motorSpeedDiff);
		clipLR(motorSpeedLeft, motorSpeedDiff, &lastSpeedLeft, &lastSpeedRight, MAX_TURN_SPEED, MAX_DRIVE_ACCEL);

		// Check if complete
		if (isCompleteTurn)
		{
			displayTextLine(4, "done w/ turn");
			isComplete = true;
			break;
		}

		// Set Motor Speeds
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

	resetMotorEncoder(arm);

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
