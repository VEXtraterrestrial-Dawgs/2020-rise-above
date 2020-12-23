#pragma config(Sensor, port1,  gyro,           sensorVexIQ_Gyro)
#pragma config(Sensor, port3,  color,          sensorVexIQ_ColorGrayscale)
#pragma config(Motor,  motor4,          leftWheels,    tmotorVexIQ, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motor7,          hDrive,        tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor8,          armLow,        tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor9,          armHigh,       tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor12,         rightWheels,   tmotorVexIQ, PIDControl, reversed, driveRight, encoder)

task main()
{
	// Start a group of datalog values
	datalogDataGroupStart();

	// Add the value 27 to datalog series 1
	datalogAddValue( 1, 27 );
	// Add the value 27 to datalog series 1
	datalogAddValue( 2, 29 );

	// End a group of datalog values
	datalogDataGroupEnd();

	// Start a group of datalog values
	datalogDataGroupStart();

	// Add the value 27 to datalog series 1
	datalogAddValue( 1, 37 );
	// Add the value 27 to datalog series 1
	datalogAddValue( 2, 39 );

	// End a group of datalog values
	datalogDataGroupEnd();

	// Start a group of datalog values
	datalogDataGroupStart();

	// Add the value 27 to datalog series 1
	datalogAddValue( 1, 47 );
	// Add the value 27 to datalog series 1
	datalogAddValue( 2, 49 );

	// End a group of datalog values
	datalogDataGroupEnd();



}
