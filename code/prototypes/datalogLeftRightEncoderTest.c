#pragma config(Sensor, port1,  gyro,           sensorVexIQ_Gyro)
#pragma config(Sensor, port3,  color,          sensorVexIQ_ColorGrayscale)
#pragma config(Motor,  motor4,          leftWheels,    tmotorVexIQ, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motor7,          hDrive,        tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor8,          armLow,        tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor9,          armHigh,       tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor12,         rightWheels,   tmotorVexIQ, PIDControl, reversed, driveRight, encoder)

task main()
{
	while(getMotorEncoder(leftWheels) < 50 && getMotorEncoder(rightWheels) < 50) {
		float leftEncoder = getMotorEncoder(leftWheels);
		float rightEncoder = getMotorEncoder(rightWheels);

		setMotorSpeed(leftWheels, 100);
		setMotorSpeed(rightWheels, 100);

		datalogDataGroupStart();
		datalogAddValueWithTimeStamp( 0, leftEncoder );
		datalogAddValueWithTimeStamp( 1, rightEncoder );
		datalogDataGroupEnd();
	}
	setMotorSpeed(leftWheels, 0);
	setMotorSpeed(rightWheels, 0);
}
