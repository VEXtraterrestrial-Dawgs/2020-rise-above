#pragma systemFile

const int WHEEL_CIRCUMFERENCE = 200; // in mm
const int DRIVE_GEAR_RATIO = 5;
const int ARRAY_LENGTH = 5;
const int THRESHOLD = 1;
const int LEFT_RIGHT_THRESHOLD = 1;
const int DRIVETRAIN_WIDTH = 50;

typedef struct {
	int targetPosition;
	int targetHeading;
	int lastEncodersLeft[ARRAY_LENGTH];
	int lastEncodersRight[ARRAY_LENGTH];
	int lastGyros[ARRAY_LENGTH];
	int lastTime;
	int writeSlot;
	int writtenSlots;
} PidObject;

// Return value says if motion is still in progress
bool PIDControl(PidObject* pid) {
	// TODO: should PIDControl be used for hDrive as well?

	// Retrieve Values
	float currentEncoderLeft = getMotorEncoder(leftWheels);
	float currentEncoderRight = getMotorEncoder(rightWheels);

	// See if we are at our target position yet
	if( currentEncoderLeft >= (pid->targetPosition - THRESHOLD) &&
		  currentEncoderLeft <= (pid->targetPosition + THRESHOLD) &&
		  currentEncoderRight >= (pid->targetPosition - THRESHOLD) &&
		  currentEncoderRight <= (pid->targetPosition + THRESHOLD) ) {
		resetMotorEncoder(leftWheels);
		resetMotorEncoder(rightWheels);
		return false;
  }

  // TODO: If we are not at our target position, we need to make corrections to our speed so we reach our target

  // If the left and right sides of the drivetrain have different encoder counts, we are not moving in a
  // straight line so we need to fix that
  if(fabs(currentEncoderLeft - currentEncoderRight) < LEFT_RIGHT_THRESHOLD) {
  	// TODO: Some sort of pid logic that adjusts the left and right speeds - look at previous left/right differences
  }

  int distanceFromTarget = pid->targetPosition - currentEncoderLeft;
  // TODO: add some basic logic that adjusts the left and right speeds based on distanceFromTarget
  // If we are driving motors inside PIDControl, we can have an internal variable for left and right speeds,
  // If driving motors from driveRobot, store left and right speeds in pid object

  // Update values of the pid object and return true
  pid->lastEncodersLeft[pid->writeSlot] = currentEncoderLeft;
  pid->lastEncodersRight[pid->writeSlot] = currentEncoderRight;
 	if(pid->writtenSlots < ARRAY_LENGTH) { pid->writtenSlots++; }
 	pid->writeSlot = (pid->writeSlot + 1) % ARRAY_LENGTH;
 	pid->lastTime = nPgmTime;

	return true;
}

void driveRobot(int targetPosition, int targetHeading)
{
	PidObject pid;
	bool stillMoving = true;

	resetMotorEncoder(leftWheels);
	resetMotorEncoder(rightWheels);
	resetGyro(gyro);

	// Given how far we want to go in millimeters, find how far we want to go in encoder units
	float encoderTarget = pid.targetPosition / (WHEEL_CIRCUMFERENCE * DRIVE_GEAR_RATIO);

	// Initialize other values of pid object
	pid.targetPosition = encoderTarget;
	pid.targetHeading = targetHeading;
	pid.writeSlot = 0;
	pid.writtenSlots = 0;

	while (stillMoving)
	{
		// TODO: Check if we've been cancelled
		stillMoving = PIDControl(pid);
		sleep(75);
	}
}

bool turnRobot(int angle) {
	int distanceMM = (DRIVETRAIN_WIDTH / 2) * angle;
	int distanceEncoders = distanceMM / (WHEEL_CIRCUMFERENCE * DRIVE_GEAR_RATIO);
	return true;
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
