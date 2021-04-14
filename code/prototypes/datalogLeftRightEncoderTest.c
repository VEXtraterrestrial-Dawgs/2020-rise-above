#pragma config(Sensor, port6,  touch,          sensorVexIQ_LED)
#pragma config(Sensor, port10, gyro,           sensorVexIQ_Gyro)
#pragma config(Motor,  motor1,          claw,          tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor4,          leftWheels,    tmotorVexIQ, PIDControl, reversed, driveLeft, encoder)
#pragma config(Motor,  motor5,          rightArm,      tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor11,         leftArm,       tmotorVexIQ, PIDControl, reversed, encoder)
#pragma config(Motor,  motor12,         rightWheels,   tmotorVexIQ, PIDControl, driveRight, encoder)

task main()
{
	datalogClear();
	setMotorEncoderUnits(encoderCounts);
	setMotorSpeed(leftWheels, 60);
	setMotorSpeed(rightWheels, 60);

	while(true) {
		int leftEncoder = round(getMotorEncoder(leftWheels));
		int rightEncoder = round(getMotorEncoder(rightWheels));
		int gyroReading  = getGyroDegrees(gyro);

		datalogDataGroupStart();
		datalogAddValueWithTimeStamp( 1, leftEncoder );
		datalogAddValue( 2, rightEncoder );
		datalogAddValue( 3, gyroReading );
		datalogDataGroupEnd();

		if (leftEncoder > 5000 && rightEncoder > 5000) {
			break;
		}

		sleep(20);
	}
	setMotorSpeed(leftWheels, 0);
	setMotorSpeed(rightWheels, 0);
}
