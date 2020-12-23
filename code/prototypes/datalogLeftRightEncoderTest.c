#pragma config(Sensor, port1,  gyro,           sensorVexIQ_Gyro)
#pragma config(Sensor, port3,  color,          sensorVexIQ_ColorGrayscale)
#pragma config(Motor,  motor4,          leftWheels,    tmotorVexIQ, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motor7,          hDrive,        tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor8,          armLow,        tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor10,          armHigh,       tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor12,         rightWheels,   tmotorVexIQ, PIDControl, reversed, driveRight, encoder)

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

		datalogAddValueWithTimeStamp( 0, leftEncoder );
		datalogAddValue( 1, rightEncoder );
		datalogAddValue( 2, gyroReading );

		if (leftEncoder > 5000 && rightEncoder > 5000) {
			break;
		}

		sleep(20);
	}
	setMotorSpeed(leftWheels, 0);
	setMotorSpeed(rightWheels, 0);
}
