#pragma config(Sensor, port1,  gyro,           sensorVexIQ_Gyro)
#pragma config(Sensor, port3,  color,          sensorVexIQ_ColorGrayscale)
#pragma config(Sensor, port6,  touch,          sensorVexIQ_LED)
#pragma config(Motor,  motor4,          leftWheels,    tmotorVexIQ, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motor7,          hDrive,        tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor8,          rightArm,      tmotorVexIQ, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motor10,         leftArm,       tmotorVexIQ, PIDControl, reversed, encoder)
#pragma config(Motor,  motor12,         rightWheels,   tmotorVexIQ, PIDControl, reversed, driveRight, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define TEST_MODE

#ifdef TEST_MODE
#define PRINT_TO_SCRN(x, y) printValues(x, y)
#else
#define PRINT_TO_SCRN(x, y)
#endif

void printValues(int left, int right)
{
	displayTextLine(0, "leftArm = %d", left);
	displayTextLine(1, "rightArm = %d", right);
}

void setLEDColor(bool tank) {
	if(tank) {
		setTouchLEDRGB(touch, 84, 122, 138);
	}
	else {
		setTouchLEDRGB(touch, 255, 177, 82);
	}
}

task main()
{
	const int THRESHOLD = 15;
	const int HDRIVEMAX = 60;
	bool tankMode = true;
	int touchCooldown = 0;

	setMotorEncoderUnits(encoderCounts);
	setLEDColor(tankMode);

	resetGyro(gyro);
	repeat(forever)
	{
		int leftJoystickY;
		int leftJoystickX;
		int rightJoystickY;
		bool lUp;
		bool lDown;
		bool rUp;
		bool rDown;
		int hDriveSpeed;
		int leftSpeed;
		int rightSpeed;
		int armSpeed;

		// First Step: This is where we retrieve all the joystick, button, and sensor values we will need

		leftJoystickY = getJoystickValue(ChA);
		leftJoystickX = getJoystickValue(ChB);
		rightJoystickY = getJoystickValue(ChD);
		lUp = (getJoystickValue(BtnLUp) == 1);
		lDown = (getJoystickValue(BtnLDown) == 1);
		rUp = (getJoystickValue(BtnRUp) == 1);
		rDown = (getJoystickValue(BtnRDown) == 1);

		if(touchCooldown > 0) {
			if(getTouchLEDValue(touch) == 0) {
				touchCooldown--;
			}
		}
		else {
			if(getTouchLEDValue(touch) == 1) {
				tankMode = !tankMode;
				touchCooldown = 3;
				setLEDColor(tankMode);
			}
		}

		// Second Step: This is where we calculate what we need to set each of the motor speeds to

		if (leftJoystickX < THRESHOLD && leftJoystickX > -THRESHOLD)
		{
			leftJoystickX = 0;
		}

		if (leftJoystickY < THRESHOLD && leftJoystickY > -THRESHOLD)
		{
			leftJoystickY = 0;
		}

		if (rightJoystickY < THRESHOLD && rightJoystickY > -THRESHOLD)
		{
			rightJoystickY = 0;
		}

		if(lUp) {
			leftSpeed = leftJoystickY;
			rightSpeed = leftJoystickY;
		}
		else {
			if(tankMode) {
				leftSpeed = ( leftJoystickY + ( 2 * leftJoystickX  ) ) / 2.75;
				rightSpeed = ( leftJoystickY - ( 2 * leftJoystickX ) ) / 2.75;
			}
			else {
				leftSpeed = round( ( leftJoystickY + leftJoystickX ) / 2.25 );
				rightSpeed = round( ( leftJoystickY - leftJoystickX ) / 2.25 );
			}
		}

  	armSpeed = rightJoystickY/2;

  	if (rUp)
  	{
  		hDriveSpeed = HDRIVEMAX;
  	}
  	else if (rDown)
  	{
  		hDriveSpeed = -HDRIVEMAX;
  	}
  	else
  	{
  		hDriveSpeed = 0;
  	}

	if (lDown) {
  		leftSpeed /= 2;
  		rightSpeed /= 2;
		hDriveSpeed /= 2;
		armSpeed /= 2;
  	}
		
	  // Third Step: This is where we set all the motor speeds to what they should be

	  setMotorSpeed(leftWheels, leftSpeed);
	  setMotorSpeed(rightWheels, rightSpeed);
	  setMotorSpeed(hDrive, hDriveSpeed);
		setMotorSpeed(leftArm, armSpeed);
		setMotorSpeed(rightArm, armSpeed);

		sleep(75);
		PRINT_TO_SCRN(round(getMotorEncoder(leftArm)), round(getMotorEncoder(rightArm)));
	}


}
