#pragma systemFile

const int WHEEL_CIRCUMFERENCE = 200; // in mm
const int DRIVE_GEAR_RATIO = 5;
const int ARRAY_LENGTH = 5;
const int THRESHOLD = 1;
const int LEFT_RIGHT_THRESHOLD = 1;
const int DRIVETRAIN_WIDTH = 132;
const int SHORT_INTERVAL = 10;
const int LONG_INTERVAL = 75;
const int ENCODER_UNITS_PER_ROTATION = 960;

typedef struct {
	int lastError;
	int lastTime;
	float Kp;
	float Ki;
	float Kd;
	float Ka; // Integral Aging Factor
	float integral;
} PidObject;
