#pragma config(Sensor, port6,  touch,          sensorVexIQ_LED)
#pragma config(Sensor, port10, gyro,           sensorVexIQ_Gyro)
#pragma config(Motor,  motor1,          claw,          tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor4,          leftWheels,    tmotorVexIQ, PIDControl, reversed, driveLeft, encoder)
#pragma config(Motor,  motor5,          rightArm,      tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor11,         leftArm,       tmotorVexIQ, PIDControl, reversed, encoder)
#pragma config(Motor,  motor12,         rightWheels,   tmotorVexIQ, PIDControl, driveRight, encoder)

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

float runTest(tMotor m1, tMotor m2, int speed, int its) {
	float startingLeft;
	float startingRight;
	unsigned long startingTime;
	float endingLeft;
	float endingRight;
	unsigned long endingTime;

	setMotorSpeed(m1, speed);
	setMotorSpeed(m2, speed);

	for(int i = 0; i < its; i++) {
		if(i == 2) {
			startingLeft = getMotorEncoder(m1);
			startingRight = getMotorEncoder(m2);
			startingTime = nPgmTime;
		}

		sleep(100);
	}

	endingLeft = getMotorEncoder(m1);
	endingRight = getMotorEncoder(m2);
	endingTime = nPgmTime;

	setMotorSpeed(m1, 0);
	setMotorSpeed(m2, 0);

	return ( ( ( ( endingLeft - startingLeft ) * 0.5 ) + ( endingRight - startingRight ) * 0.5 ) ) / ( endingTime - startingTime );
}

task main()
{
	float speeds[18];
	int speedCounter = 0;

	setMotorEncoderUnits(encoderCounts);
	setTouchLEDRGB(touch, 111, 160, 237);

	for(int s = 20; s <= 100; s += 10) {
		displayTextLine(1, "Starting");
		displayTextLine(2, "Drive Speed %d", s);
		waitForLED();
		displayTextLine(1, "Running");
		speeds[speedCounter++] = runTest(leftWheels, rightWheels, s, 20);
	}

	for(int v = 20; v <= 100; v += 10) {
		displayTextLine(1, "Starting");
		displayTextLine(2, "Arm Speed %d", v);
		waitForLED();
		displayTextLine(1, "Running");
		speeds[speedCounter++] = runTest(leftArm, rightArm, v, 10);
		setMotorTarget(leftArm, 0, 30);
		setMotorTarget(rightArm, 0, 30);
		waitUntilMotorStop(leftArm);
		waitUntilMotorStop(rightArm);
	}

	for(int l = 0; l < 18; l++) {
		displayTextLine(1, "Entry %d", l);
		displayTextLine(2, "%f", speeds[l]);
		waitForLED();
	}
}
