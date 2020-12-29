#pragma systemFile

const int WHEEL_CIRCUMFERENCE = 200; // in mm
const int DRIVE_GEAR_RATIO = 5;
const int H_DRIVE_GEAR_RATIO = 3;
const int ARM_GEAR_RATIO = 0.2;
const int ARRAY_LENGTH = 5;
const int THRESHOLD = 5;
const int THRESHOLD_ARM = 5;
const int LEFT_RIGHT_THRESHOLD = 1;
const int DRIVETRAIN_WIDTH = 132;
const int SHORT_INTERVAL = 10;
const int LONG_INTERVAL = 75;
const int ENCODER_UNITS_PER_ROTATION = 960;
const int MAX_DRIVE_SPEED = 90;
const int MAX_DRIVE_ACCEL = 20;
const int MAX_DRIVE_DIFFERENCE = 10;
const int MAX_TURN_SPEED = 60;
const int MAX_ARM_SPEED = 60;
const int MAX_ARM_ACCEL = 15;


typedef struct {
	int lastError; // Last Iteration's Error Value
	int lastTime; // Time of Last Iteration
	int controllerIndex;  // For Datalog
	float Kp; // Proportional Coefficient
	float Ki; // Integral Coefficient
	float Kd; // Derivative Coefficient
	float Ka; // Aging Factor for Integral
	float integral; // Integral Value
} PidObject;
