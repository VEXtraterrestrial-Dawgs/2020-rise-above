#pragma config(Sensor, port6,  touch,          sensorVexIQ_LED)
#pragma config(Sensor, port9,  gyroBack,       sensorVexIQ_Gyro)
#pragma config(Sensor, port10, gyroFront,      sensorVexIQ_Gyro)
#pragma config(Motor,  motor1,          claw,          tmotorVexIQ, PIDControl, reversed, encoder)
#pragma config(Motor,  motor4,          leftWheels,    tmotorVexIQ, PIDControl, reversed, driveLeft, encoder)
#pragma config(Motor,  motor5,          rightArm,      tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor11,         leftArm,       tmotorVexIQ, PIDControl, reversed, encoder)
#pragma config(Motor,  motor12,         rightWheels,   tmotorVexIQ, PIDControl, driveRight, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "PIDControl.c"
#include "robotParams.h"

#define TEST_MODE

#ifdef TEST_MODE
#define PRINT_TO_SCRN(x, y) printValues(x, y)
#else
#define PRINT_TO_SCRN(x, y)
#endif

void printValues(int d, int g)
{
	int driveMM = d / ENC_UNITS_PER_MM;
	displayTextLine(2, "drive: %d", driveMM);
	displayTextLine(3, "gyro: %d", g);
}

void setLEDColor(bool tank) {
	if(tank) {
		setTouchLEDRGB(touch, 84, 122, 138);
		displayTextLine(1, "Tank Mode Enabled");
	}
	else {
		setTouchLEDRGB(touch, 255, 177, 82);
		displayTextLine(1, "Arcade Mode Enabled");
	}
}

bool isCancelled() {
	return false;
}

task main()
{
	const int CLAW_SPEED = 50;
	const int THRESHOLD = 15;
	bool tankMode = true;
	int touchCooldown = 0;
	StuckDetector clawStuck;

	initStuckDetector(&clawStuck, claw, CLAW_STUCK_THRESHOLD);

	setMotorEncoderUnits(encoderCounts);

	setTouchLEDRGB(touch, 255, 247, 0);
	displayTextLine(1, "Press LED to Start");
	waitForLED();
	calibrateGyros();
	moveClaw(0, CLOSE, CLAW_SPEED_SLOW, CLAW_SPEED_SLOW);
	resetMotorEncoder(claw);
	OPEN_CLAW();

	setLEDColor(tankMode);

	gyroReset();
	repeat(forever)
	{
		int leftJoystickY;
		int leftJoystickX;
		int rightJoystickY;
		bool lUp;
		bool lDown;
		bool rUp;
		bool rDown;
		int leftSpeed;
		int rightSpeed;
		int armSpeed;
		int clawSpeed;
		int armEncoder;

		// First Step: This is where we retrieve all the joystick, button, and sensor values we will need

		leftJoystickY = getJoystickValue(ChA);
		leftJoystickX = getJoystickValue(ChB);
		rightJoystickY = getJoystickValue(ChD);
		lUp = (getJoystickValue(BtnLUp) == 1);
		lDown = (getJoystickValue(BtnLDown) == 1);
		rUp = (getJoystickValue(BtnRUp) == 1);
		rDown = (getJoystickValue(BtnRDown) == 1);
		armEncoder = getMotorEncoder(leftArm);

		if(touchCooldown > 0) {
			if(getTouchLEDValue(touch) == 0) {
				touchCooldown--;
			}
		}
		else {
			if(getTouchLEDValue(touch) == 1) {
				tankMode = !tankMode;
				touchCooldown = 3;
				setLEDColor(tankMode);
			}
		}

		// Second Step: This is where we calculate what we need to set each of the motor speeds to

		if (abs(leftJoystickX) < THRESHOLD)
		{
			leftJoystickX = 0;
		}

		if (abs(leftJoystickY) < THRESHOLD)
		{
			leftJoystickY = 0;
		}

		if (abs(rightJoystickY) < THRESHOLD)
		{
			rightJoystickY = 0;
		}

		if(lUp) {
			leftSpeed = leftJoystickY;
			rightSpeed = leftJoystickY;
		}
		else {
			if(tankMode) {
				leftSpeed = ( leftJoystickY + ( 2 * leftJoystickX  ) ) / 3;
				rightSpeed = ( leftJoystickY - ( 2 * leftJoystickX ) ) / 3;
			}
			else {
				leftSpeed = round( ( leftJoystickY + leftJoystickX ) / 2 );
				rightSpeed = round( ( leftJoystickY - leftJoystickX ) / 2 );
			}
		}

		if(rightJoystickY != 0) {
			if(armEncoder >= ARM_HIGH || armEncoder <= 0) playNote(noteF, octave2, 7);
		}

		armSpeed = rightJoystickY;

		if (lDown) {
			leftSpeed /= 2;
			rightSpeed /= 2;
			armSpeed /= 2;
		}

		if (rDown) {
			clawSpeed = -CLAW_SPEED;
		}
		else if (rUp) {
			clawSpeed = CLAW_SPEED;
		}
		else {
			clawSpeed = 0;
		}

		if(rDown || rUp) {
			if(isStuck(&clawStuck)) {
				playNote(noteF, octave2, 7);
				clawSpeed = 0;
			}
		}
		else {
			resetStuckDetector(&clawStuck);
		}

		// Third Step: This is where we set all the motor speeds to what they should be

		setMotorSpeed(leftWheels, convertToMotorSpeed(leftSpeed));
		setMotorSpeed(rightWheels, convertToMotorSpeed(rightSpeed));
		setMotorSpeed(leftArm, convertToMotorSpeed(armSpeed));
		setMotorSpeed(rightArm, convertToMotorSpeed(armSpeed));
		setMotorSpeed(claw, convertToMotorSpeed(clawSpeed));

		sleep(75);

		int gyroD = -gyroValue();

		if (gyroD < -180) {
			gyroD = ((( gyroD - 180 ) % 360 ) + 180 );
		}
		else if ( gyroD > 180 ) {
			gyroD = ((( gyroD + 180 ) % 360 ) - 180 );
		}
		PRINT_TO_SCRN(round(getMotorEncoder(leftWheels)), gyroD);
	}
}
