#pragma systemFile

const int WHEEL_CIRCUMFERENCE = 200; // in mm
const int DRIVE_GEAR_RATIO = 3;
const int H_DRIVE_GEAR_RATIO = 3;
const int ARM_GEAR_RATIO = 0.2;
const int ARRAY_LENGTH = 5;
const int THRESHOLD = 7;
const int THRESHOLD_ARM = 5;
const int LEFT_RIGHT_THRESHOLD = 3;
const int CLOSE_THRESHOLD = 15;
const int DRIVETRAIN_WIDTH = 132;
const int SHORT_INTERVAL = 50;
const int LONG_INTERVAL = 75;
const int ENCODER_UNITS_PER_ROTATION = 960;
const int MAX_DRIVE_SPEED = 70;
const int MAX_DRIVE_ACCEL = 30;
const int MAX_DRIVE_DIFFERENCE = 60;
const int MAX_TURN_SPEED = 70;
const int MAX_ARM_SPEED = 90;
const int MAX_ARM_ACCEL = 15;
const int MOTOR_RANGE = 45;
const int MOTOR_LOW = 25;
const int ARM_TOP_HIGH = 1820;
const int ARM_BOTTOM_HIGH = 1490;
const int ARM_TOP_STACK = 1175;
const int ARM_BOTTOM_STACK = 742;
const float TURN_AVG_KA = 0.8;
const float ENC_UNITS_PER_MM = (float)ENCODER_UNITS_PER_ROTATION / ((float)WHEEL_CIRCUMFERENCE * DRIVE_GEAR_RATIO);
const float ENC_UNITS_PER_DEGREE = 0.5 * DRIVETRAIN_WIDTH * degreesToRadians(1) * ENC_UNITS_PER_MM;

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

// MUST be called inside of a bool function that can return when any operation is cancelled.
#define RUN_COMMAND(what, howMuch) if (what(howMuch) ==  false) { return false; }

#ifdef TEST_MODE
#define CLEAR_LOG() datalogClear()
#define LOG(x,y) datalogAddValue(x,y)
#define LOG_WITH_TIME(x,y) datalogAddValueWithTimeStamp(x,y)
#else
#define CLEAR_LOG()
#define LOG(x,y)
#define LOG_WITH_TIME(x,y)
#endif
