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

bool PIDControl(PidObject* pid, int target, int current, int threshold, int* power) {
	int error = target - current;
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
	datalogAddValueWithTimeStamp( 0, pid->controllerIndex );
	datalogAddValue( 1, error );
	datalogAddValue( 2, round(pid->integral*1000) );
	datalogAddValue( 3, round(derivative*1000) );
	datalogAddValue( 4, round(getGyroDegrees(gyro)) );

	// Returns a motor speed
	*power = round( (error * pid->Kp) + (pid->integral * pid->Ki) + (derivative * pid->Kd) );

	return (error < threshold);
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

	PIDInit(&controllerLeft, 1, encoderTarget, 0.75, 0, 0, 0.95);
	PIDInit(&controllerRight, 2, 0, 1.2, 0, 0, 0.995);

	while (!isCancelled())
	{
		int motorSpeedLeft;
		int motorSpeedRight;
		int leftEncoder = round(getMotorEncoder(leftWheels));
		int rightEncoder = round(getMotorEncoder(rightWheels));
		bool hasReached;
		bool isStraight;

		hasReached = PIDControl(&controllerLeft, encoderTarget, leftEncoder, THRESHOLD, &motorSpeedLeft);
		motorSpeedLeft = clip(motorSpeedLeft, lastSpeedLeft, MAX_DRIVE_SPEED, MAX_DRIVE_ACCEL);
		lastSpeedLeft = motorSpeedLeft;

		isStraight = PIDControl(&controllerRight, leftEncoder, rightEncoder, THRESHOLD, &motorSpeedRight);
		motorSpeedRight = clip(motorSpeedLeft + motorSpeedRight, lastSpeedRight, MAX_DRIVE_SPEED + MAX_DRIVE_DIFFERENCE, MAX_DRIVE_ACCEL + MAX_DRIVE_DIFFERENCE);
		lastSpeedRight = motorSpeedRight;

		if (hasReached && isStraight)
		{
			isComplete = true;
			break;
		}

		setMotorSpeed(leftWheels, motorSpeedLeft);
		setMotorSpeed(rightWheels, motorSpeedRight);
		sleep(LONG_INTERVAL);
	}

	setMotorSpeed(leftWheels, 0);
	setMotorSpeed(rightWheels, 0);
	return isComplete;
}

bool turnRobot(int angle) {
	PidObject controllerLeftTurn;
	PidObject controllerRightTurn;
	float angleRadians = degreesToRadians(angle);
	float distanceMM = DRIVETRAIN_WIDTH * 0.5 * angleRadians;
	int distanceEncoders = round((distanceMM * ENCODER_UNITS_PER_ROTATION) / (WHEEL_CIRCUMFERENCE * DRIVE_GEAR_RATIO));
	bool isComplete = false;

	resetMotorEncoder(leftWheels);
	resetMotorEncoder(rightWheels);
	resetGyro(gyro);

	PIDInit(&controllerLeftTurn, 3, -distanceEncoders, 2, 0, 0, 0.95);
	PIDInit(&controllerRightTurn, 4, 0, 1, 0, 0, 0.95);

	int lastSpeedLeft = 0;
	int lastSpeedRight = 0;

	while (!isCancelled())
	{
		// Retrieve Sensor Values
		int encoderLeft = round(getMotorEncoder(leftWheels));
		int encoderRight = round(getMotorEncoder(rightWheels));
		float gyroValue = degreesToRadians(getGyroDegreesFloat(gyro));

		// Correct Sensor Values
		int heading = round(( DRIVETRAIN_WIDTH * 0.5 * gyroValue * ENCODER_UNITS_PER_ROTATION ) /
		                ( WHEEL_CIRCUMFERENCE * DRIVE_GEAR_RATIO ));

		// Calculate Motor Speeds
		bool isCompleteLeft;
		bool isCompleteRight;
		int motorSpeedLeft;
		int motorSpeedRight;

		isCompleteLeft = PIDControl(&controllerLeftTurn, -distanceEncoders, heading, THRESHOLD, &motorSpeedLeft);
		motorSpeedLeft = clip(motorSpeedLeft, lastSpeedLeft, MAX_TURN_SPEED, MAX_DRIVE_ACCEL);
		lastSpeedLeft = motorSpeedLeft;

		isCompleteRight = PIDControl(&controllerRightTurn, -encoderLeft, encoderRight, THRESHOLD, &motorSpeedRight);
		motorSpeedRight = clip(motorSpeedRight + motorSpeedLeft, lastSpeedRight, MAX_TURN_SPEED + MAX_DRIVE_DIFFERENCE, MAX_DRIVE_ACCEL + MAX_DRIVE_DIFFERENCE);
		lastSpeedRight = motorSpeedRight;

		// Check if complete
		if (isCompleteRight && isCompleteLeft)
		{
			isComplete = true;
			playSound(soundTada);
			break;
		}

		// Set Motor Speeds
		setMotorSpeed(leftWheels, -motorSpeedLeft);
		setMotorSpeed(rightWheels, motorSpeedRight);
		playSound(soundSiren2);
		sleep(SHORT_INTERVAL);
	}

	setMotorSpeed(leftWheels, 0);
	setMotorSpeed(rightWheels, 0);
	return isComplete;
}

bool moveHDrive(int distance) {
	PidObject controllerHDrive;
	bool isComplete;
	int encoderTarget = (distance / (WHEEL_CIRCUMFERENCE * H_DRIVE_GEAR_RATIO))*ENCODER_UNITS_PER_ROTATION;

	resetMotorEncoder(hDrive);

	PIDInit(&controllerHDrive, 5, encoderTarget, 0.8, 0, 0, 0.95);

	int lastSpeed = 0;

	while(!isCancelled()) {
		int motorSpeed;

		isComplete = PIDControl(&controllerHDrive, encoderTarget, getMotorEncoder(hDrive), THRESHOLD, &motorSpeed);
		motorSpeed = clip(motorSpeed, lastSpeed, MAX_DRIVE_SPEED, MAX_DRIVE_ACCEL);
		lastSpeed = motorSpeed;

		if(isComplete) {
			break;
		}

		setMotorSpeed(hDrive, motorSpeed);
		sleep(LONG_INTERVAL);
	}

	setMotorSpeed(hDrive, 0);
	return isComplete;
}

bool moveArm(tMotor arm, int height) {
	PidObject controllerArm;
	bool isComplete;

	resetMotorEncoder(arm);

	PIDInit(&controllerArm, 6, height, 0.5, 0, 0, 0.95);

	int lastSpeed = 0;

	while(!isCancelled()) {
		int motorSpeed;

		isComplete = PIDControl(&controllerArm, height, getMotorEncoder(arm), THRESHOLD_ARM, &motorSpeed);
		motorSpeed = clip(motorSpeed, lastSpeed, MAX_ARM_SPEED, MAX_ARM_ACCEL);
		lastSpeed = motorSpeed;

		if(isComplete) {
			break;
		}

		setMotorSpeed(arm, motorSpeed);
		sleep(SHORT_INTERVAL);
	}

	setMotorSpeed(arm, 0);
	return isComplete;
}
