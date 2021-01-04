#pragma config(Sensor, port1,  gyro,           sensorVexIQ_Gyro)
#pragma config(Sensor, port3,  color,          sensorVexIQ_ColorGrayscale)
#pragma config(Sensor, port6,  touch,          sensorVexIQ_LED)
#pragma config(Motor,  motor4,          leftWheels,    tmotorVexIQ, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motor7,          hDrive,        tmotorVexIQ, PIDControl, reversed, encoder)
#pragma config(Motor,  motor8,          armLow,        tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor10,         armHigh,       tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor12,         rightWheels,   tmotorVexIQ, PIDControl, reversed, driveRight, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define TEST_MODE
#include "autonomousfn.c"

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
	setTouchLEDColor(touch, moveTopArm(453)?colorDarkBlue:colorRed);
	setTouchLEDColor(touch, moveLowerArm(225)?colorDarkBlue:colorRed);
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

bool autonomousStrategy() {
	RUN_COMMAND(moveHDrive, 255);
	RUN_COMMAND(moveHDrive, -255);
	RUN_COMMAND(driveRobot, 365);
	RUN_COMMAND(turnRobot, 90);
	RUN_COMMAND(driveRobot, 410);
	// raise top arm
	RUN_COMMAND(driveRobot, 55);
	// raise bottom arm
	RUN_COMMAND(moveHDrive, 200);
	// put down risers
	return true;
}

task main()
{
	calibrateGyro();
	setMotorBrakeMode(armHigh, motorHold);
	setMotorBrakeMode(armLow, motorHold);
	CLEAR_LOG();
	setMotorEncoderUnits(encoderCounts);
	setTouchLEDRGB(touch, 84, 122, 138);

	testSequence();
}
