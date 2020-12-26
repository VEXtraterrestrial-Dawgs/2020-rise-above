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

	controllerLeft.Kp = 0.8;
	controllerLeft.Ki = 0;
	controllerLeft.Kd = 0;
	controllerLeft.Ka = 0.95;
	controllerLeft.controllerIndex = 1;

	controllerRight.Kp = 1;
	controllerRight.Ki = 0;
	controllerRight.Kd = 0;
	controllerRight.Ka = 0.995;
	controllerRight.controllerIndex = 2;

	// Given how far we want to go in millimeters, find how far we want to go in encoder units
	int encoderTarget = (distanceInMM / (WHEEL_CIRCUMFERENCE * DRIVE_GEAR_RATIO)) * ENCODER_UNITS_PER_ROTATION;
	int lastSpeedLeft = 0;
	int lastSpeedRight = 0;

	while (!isCancelled())
	{
		int motorSpeedLeft;
		int motorSpeedRight;
		int leftEncoder = round(getMotorEncoder(leftWheels));
		int rightEncoder = round(getMotorEncoder(rightWheels));
		bool isLeftComplete;
		bool isRightComplete;

		isLeftComplete = PIDControl(&controllerLeft, encoderTarget, leftEncoder, THRESHOLD, &motorSpeedLeft);
		motorSpeedLeft = clip(motorSpeedLeft, lastSpeedLeft, MAX_DRIVE_SPEED, MAX_DRIVE_ACCEL);
		lastSpeedLeft = motorSpeedLeft;

		isRightComplete = PIDControl(&controllerRight, leftEncoder, rightEncoder, THRESHOLD, &motorSpeedRight);
		motorSpeedRight = clip(motorSpeedLeft + motorSpeedRight, lastSpeedRight, MAX_DRIVE_SPEED + MAX_DRIVE_DIFFERENCE, MAX_DRIVE_ACCEL + MAX_DRIVE_DIFFERENCE);
		lastSpeedRight = motorSpeedRight;

		if (isLeftComplete && isRightComplete)
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
	float distanceMM = (DRIVETRAIN_WIDTH / 2) * angleRadians;
	float distanceEncoders = (distanceMM / (WHEEL_CIRCUMFERENCE * DRIVE_GEAR_RATIO))*ENCODER_UNITS_PER_ROTATION;
	bool isComplete = false;

	resetMotorEncoder(leftWheels);
	resetMotorEncoder(rightWheels);
	resetGyro(gyro);

	controllerLeftTurn.Kp = 2;
	controllerLeftTurn.Ki = 0;
	controllerLeftTurn.Kd = 0;
	controllerLeftTurn.Ka = 0.95;

	controllerRightTurn.Kp = 1;
	controllerRightTurn.Ki = 0;
	controllerRightTurn.Kd = 0;
	controllerRightTurn.Ka = 0.995;

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
		clip(motorSpeedLeft, lastSpeedLeft, MAX_TURN_SPEED, MAX_DRIVE_ACCEL);
		lastSpeedLeft = motorSpeedLeft;

		isCompleteRight = PIDControl(&controllerRightTurn, -encoderLeft, encoderRight, THRESHOLD, &motorSpeedRight);
		clip(motorSpeedRight - motorSpeedLeft, lastSpeedRight, MAX_TURN_SPEED + MAX_DRIVE_DIFFERENCE, MAX_DRIVE_ACCEL + MAX_DRIVE_DIFFERENCE);
		lastSpeedRight = motorSpeedRight;

		// Check if complete
		if (isCompleteRight && isCompleteLeft)
		{
			isComplete = true;
			break;
		}

		// Set Motor Speeds
		setMotorSpeed(leftWheels, motorSpeedLeft);
		setMotorSpeed(rightWheels, motorSpeedRight);
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

	controllerHDrive.Kp = 0.8;
	controllerHDrive.Ki = 0;
	controllerHDrive.Kd = 0;
	controllerHDrive.Ka = 0.95;

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

	controllerArm.Kp = 0.5;
	controllerArm.Ki = 0;
	controllerArm.Kd = 0;
	controllerArm.Ka = 0.95;

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
