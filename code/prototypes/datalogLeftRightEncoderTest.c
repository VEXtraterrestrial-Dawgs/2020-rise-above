#pragma config(Sensor, port1,  gyro,           sensorVexIQ_Gyro)
#pragma config(Sensor, port3,  color,          sensorVexIQ_ColorGrayscale)
#pragma config(Motor,  motor4,          leftWheels,    tmotorVexIQ, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motor7,          hDrive,        tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor8,          armLow,        tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor9,          armHigh,       tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor12,         rightWheels,   tmotorVexIQ, PIDControl, reversed, driveRight, encoder)

task main()
{
	datalogClear();
	setMotorEncoderUnits(encoderCounts);
	setMotorSpeed(leftWheels, 100);
	setMotorSpeed(rightWheels, 100);

	while(true) {
		int leftEncoder = getMotorEncoder(leftWheels);
		int rightEncoder = getMotorEncoder(rightWheels);
		int gyroReading  = getGyroDegrees(gyro);

		datalogDataGroupStart();
		datalogAddValueWithTimeStamp( 0, leftEncoder );
		datalogAddValue( 1, rightEncoder );
		datalogAddValue( 2, gyroReading );
		datalogDataGroupEnd();

		if (leftEncoder > 50 && rightEncoder > 50) {
			break;
		}

		sleep(20);
	}
	setMotorSpeed(leftWheels, 0);
	setMotorSpeed(rightWheels, 0);
}
