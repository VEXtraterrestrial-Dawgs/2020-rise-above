#pragma config(Sensor, port1,  gyro,           sensorVexIQ_Gyro)
#pragma config(Sensor, port3,  color,          sensorVexIQ_ColorGrayscale)
#pragma config(Sensor, port6,  touch,          sensorVexIQ_LED)
#pragma config(Motor,  motor4,          leftWheels,    tmotorVexIQ, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motor7,          hDrive,        tmotorVexIQ, PIDControl, reversed, encoder)
#pragma config(Motor,  motor8,          rightArm,      tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor10,         leftArm,       tmotorVexIQ, PIDControl, reversed, encoder)
#pragma config(Motor,  motor12,         rightWheels,   tmotorVexIQ, PIDControl, reversed, driveRight, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "autonomousfn.c"

#define TEST_MODE

bool testSequence() {
	displayTextLine(1, "Starting");
	displayTextLine(2, "H-Drive");
	waitForLED();
	displayTextLine(1, "Running");
	setTouchLEDColor(touch, colorYellow);
	setTouchLEDColor(touch, moveHDrive(200)?colorDarkBlue:colorRed);
	displayTextLine(1, "Starting");
	displayTextLine(2, "Arm");
	waitForLED();
	displayTextLine(1, "Running");
	setTouchLEDColor(touch, colorYellow);
	setTouchLEDColor(touch, moveArm(225)?colorDarkBlue:colorRed);
	displayTextLine(1, "Starting");
	displayTextLine(2, "Drive");
	waitForLED();
	displayTextLine(1, "Running");
	setTouchLEDColor(touch, colorYellow);
	setTouchLEDColor(touch, driveRobot(300)?colorDarkBlue:colorRed);
	displayTextLine(1, "Starting");
	displayTextLine(2, "Turn");
	waitForLED();
	displayTextLine(1, "Running");
	setTouchLEDColor(touch, colorYellow);
	setTouchLEDColor(touch, turnRobot(90)?colorDarkBlue:colorRed);
	displayTextLine(1, "Finished");
	displayTextLine(2, "Tests");
	return true;
}

bool autonomousStrategy(int row) {
	displayTextLine(1, "Starting Row: %d", row);
	waitForLED();
	setTouchLEDRGB(touch, 255, 240, 122);

	// H_DRIVE and TURN must have row as the second parameter

	H_DRIVE(780, row);
	H_DRIVE(-581, row);
 	DRIVE(189);
	sleep(50);
	TURN(90, row);
	DRIVE(400);
	MOVE_ARM(ARM_HIGH);
	H_DRIVE(200, row);
	MOVE_ARM(ARM_STACK);
	DRIVE(-50);
	MOVE_ARM(0);
	TURN(-90, row);
	DRIVE(755);
	H_DRIVE(250, row);
	H_DRIVE(-660, row);
	H_DRIVE(410, row);
	DRIVE(850);
	H_DRIVE(250, row);
	H_DRIVE(-250, row);
	TURN(90, row);
	H_DRIVE(200, row);
	DRIVE(85);
	MOVE_ARM(ARM_HIGH);
	H_DRIVE(-200, row);
	MOVE_ARM(ARM_STACK);
	DRIVE(-100);
	MOVE_ARM(0);

	displayTextLine(1, "Finished Program");
	setTouchLEDRGB(touch, 54, 255, 90);

	return true;
}

task main()
{
	displayTextLine(1, "Press the LED to Calibrate Gyro");
	setTouchLEDRGB(touch, 255, 17, 0);
	waitForLED();
	calibrateGyro();
	setMotorBrakeMode(leftArm, motorHold);
	setMotorBrakeMode(rightArm, motorHold);
	CLEAR_LOG();
	setMotorEncoderUnits(encoderCounts);
	resetMotorEncoder(leftArm);
	resetMotorEncoder(rightArm);
	setTouchLEDRGB(touch, 84, 122, 138);

	//testSequence();
	autonomousStrategy(PURPLE);
	autonomousStrategy(TEAL);
}
