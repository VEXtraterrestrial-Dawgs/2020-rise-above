#pragma systemFile

#include "robotParams.h"

#define TEST_MODE

#ifdef TEST_MODE
#define CLEAR_LOG() datalogClear()
#define LOG(x,y) datalogAddValue(x,y)
#define LOG_WITH_TIME(x,y) datalogAddValueWithTimeStamp(x,y)
#else
#define CLEAR_LOG()
#define LOG(x,y)
#define LOG_WITH_TIME(x,y)
#endif

const int MAX_DRIVE_DIFFERENCE = 60;

typedef struct {
	int lastError; // Last Iteration's Error Value
	int lastTime; // Time of Last Iteration
	int controllerIndex;  // For Datalog
	float Kp; // Proportional Coefficient
	float Ki; // Integral Coefficient
	float Kd; // Derivative Coefficient
	float Ka; // Aging Factor for Integral
	float integral; // Integral Value
} PidObject;

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
	LOG( 4, gyroValue() );

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
