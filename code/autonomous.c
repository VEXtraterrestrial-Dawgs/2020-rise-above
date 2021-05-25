#pragma config(Sensor, port7,  touch,          sensorVexIQ_LED)
#pragma config(Sensor, port9,  gyroBack,       sensorVexIQ_Gyro)
#pragma config(Sensor, port10, gyroFront,      sensorVexIQ_Gyro)
#pragma config(Motor,  motor1,          claw,          tmotorVexIQ, PIDControl, reversed, encoder)
#pragma config(Motor,  motor4,          leftWheels,    tmotorVexIQ, PIDControl, reversed, driveLeft, encoder)
#pragma config(Motor,  motor5,          rightArm,      tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor11,         leftArm,       tmotorVexIQ, PIDControl, reversed, encoder)
#pragma config(Motor,  motor12,         rightWheels,   tmotorVexIQ, PIDControl, driveRight, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "autonomousfn.c"

bool testSequence() {
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
	displayTextLine(1, "Starting");
	displayTextLine(2, "Arm");
	waitForLED();
	displayTextLine(1, "Running");
	setTouchLEDColor(touch, colorYellow);
	setTouchLEDColor(touch, moveArm(ARM_STACK)?colorDarkBlue:colorRed);
	displayTextLine(1, "Starting");
	displayTextLine(2, "Claw Close");
	waitForLED();
	displayTextLine(1, "Running");
	setTouchLEDColor(touch, colorYellow);
	setTouchLEDColor(touch, moveClaw(CLAW_CLOSE, CLOSE, CLAW_SPEED_FAST, CLAW_SPEED_SLOW)?colorDarkBlue:colorRed);
	displayTextLine(1, "Starting");
	displayTextLine(2, "Claw Open");
	waitForLED();
	displayTextLine(1, "Running");
	setTouchLEDColor(touch, colorYellow);
	setTouchLEDColor(touch, moveClaw(CLAW_OPEN, OPEN, CLAW_SPEED_FAST, CLAW_SPEED_SLOW)?colorDarkBlue:colorRed);
	displayTextLine(1, "Finished");
	displayTextLine(2, "Tests");
	return true;
}

bool autonomousStrategy(int row) {

	displayTextLine(1, "Starting %s Row", (row == 1) ? "Purple" : "Teal");
	waitForLED();
	setTouchLEDRGB(touch, 255, 240, 122);
	CLOSE_CLAW_C();
	MOVE_ARM(0);

	// DRIVE values are in millimeters
	// TURN must have row as the second parameter, values in degrees
	// OPEN_CLAW_C and CLOSE_CLAW_C take no parameters, but must have _C to enable cancelling

	// Pull out riser
	DRIVE(430);
	TURN(90, row);
	OPEN_CLAW_C();
	DRIVE(565);
	CLOSE_CLAW_C();
	DRIVE(-580);
	OPEN_CLAW_C();
	MOVE_ARM(ARM_LIFT);
	DRIVE(249);
	CLOSE_CLAW_C();
	MOVE_ARM(ARM_HIGH);

	// Push in Riser and Stack
	TURN(-90, row);
	DRIVE(359);
	TURN(90, row);
	DRIVE(300);
	MOVE_ARM(ARM_STACK);
	OPEN_CLAW_C();
	DRIVE(-300);

	// Push in Last Riser
	TURN(-90, row);
	DRIVE(685);
	TURN(90, row);
	DRIVE(300);
	DRIVE(-200);

	// Last Stack
	DRIVE(-1226);
	TURN(90, row);
	DRIVE(400);
	TURN(90, row);
	MOVE_ARM(ARM_LIFT);
	DRIVE(275);
	CLOSE_CLAW_C();
	MOVE_ARM(ARM_HIGH);
	DRIVE(-338);

	// Push in Final Riser and Stack
	TURN(20, row);
	DRIVE(100);
	TURN(-20, row);
	DRIVE(100);
	MOVE_ARM(ARM_STACK);
	OPEN_CLAW_C();

	displayTextLine(1, "Finished %s Side", (row == 1) ? "Purple" : "Teal");
	setTouchLEDRGB(touch, 54, 255, 90);

	return true;
}

task main()
{
	displayTextLine(1, "Press the LED to Calibrate Gyro");
	setTouchLEDRGB(touch, 255, 17, 0);
	waitForLED();
	calibrateGyros();
	setMotorBrakeMode(leftArm, motorHold);
	setMotorBrakeMode(rightArm, motorHold);
	CLEAR_LOG();
	setMotorEncoderUnits(encoderCounts);
	resetMotorEncoder(leftArm);
	resetMotorEncoder(rightArm);
	setTouchLEDRGB(touch, 84, 122, 138);

	moveClaw(0, CLOSE, CLAW_SPEED_SLOW, CLAW_SPEED_SLOW);
	resetMotorEncoder(claw);
	OPEN_CLAW();

//	testSequence();
	autonomousStrategy(PURPLE);
  setTouchLEDRGB(touch, 187, 237, 235);
	autonomousStrategy(TEAL);
}
