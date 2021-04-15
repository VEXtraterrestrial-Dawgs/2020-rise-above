#pragma config(Sensor, port6,  touch,          sensorVexIQ_LED)
#pragma config(Sensor, port10, gyro,           sensorVexIQ_Gyro)
#pragma config(Motor,  motor1,          claw,          tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor4,          leftWheels,    tmotorVexIQ, PIDControl, reversed, driveLeft, encoder)
#pragma config(Motor,  motor5,          rightArm,      tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor11,         leftArm,       tmotorVexIQ, PIDControl, reversed, encoder)
#pragma config(Motor,  motor12,         rightWheels,   tmotorVexIQ, PIDControl, driveRight, encoder)

typedef struct {
	unsigned long timestamp;
	int leftEncoder;
	int rightEncoder;
} Datapoint;

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

void runTest(tMotor m1, tMotor m2, int speed, int its, Datapoint* dataset) {
	setMotorSpeed(m1, speed);
	setMotorSpeed(m2, speed);

	for(int i = 0; i < its; i++) {
		dataset[i].timestamp = nPgmTime;
		dataset[i].leftEncoder = round(getMotorEncoder(m1));
		dataset[i].rightEncoder = round(getMotorEncoder(m2));
		
		sleep(100);
	}

	setMotorSpeed(m1, 0);
	setMotorSpeed(m2, 0);
}

task main()
{
	Datapoint data[270];

	setMotorEncoderUnits(encoderCounts);
	setTouchLEDRGB(touch, 111, 160, 237);

	for(int s = 20; s <= 100; s += 10) {
		displayTextLine(1, "Starting");
		displayTextLine(2, "Drive Speed %d", s);
		waitForLED();
		displayTextLine(1, "Running");
		runTest(leftWheels, rightWheels, s, 20, data + ( ( s - 20 ) * 2 ) );
	}

	for(int v = 20; v <= 100; v += 10) {
		displayTextLine(1, "Starting");
		displayTextLine(2, "Arm Speed %d", v);
		waitForLED();
		displayTextLine(1, "Running");
		runTest(leftArm, rightArm, v, 10, data + ( v + 160 ) );
	}
}
