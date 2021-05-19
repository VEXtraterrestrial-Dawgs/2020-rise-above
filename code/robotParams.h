#pragma systemFile

const float ARM_GEAR_RATIO = 1/15;
const int ARM_GEAR_SIZE = 60;
const float CLAW_GEAR_RATIO = 1/3;
const int CLAW_GEAR_SIZE = 36;
const float DRIVE_GEAR_RATIO = 3;
const int DRIVE_GEAR_SIZE = 12;
const int WHEEL_CIRCUMFERENCE = 200; // in mm
const int DRIVETRAIN_WIDTH = 206;
const int ENCODER_UNITS_PER_ROTATION = 960;
const int ARM_HIGH = 2570;
const int ARM_STACK = 1770;
const int ARM_LIFT = 270;
const float ENC_UNITS_PER_MM = (float)ENCODER_UNITS_PER_ROTATION / ((float)WHEEL_CIRCUMFERENCE * DRIVE_GEAR_RATIO);
const float ENC_UNITS_PER_DEGREE = 0.5 * DRIVETRAIN_WIDTH * degreesToRadians(1) * ENC_UNITS_PER_MM;
const int MOTOR_RANGE = 60;
const int MOTOR_LOW = 20;
const int CLAW_STUCK_THRESHOLD = 20;
const int CLAW_SPEED_SLOW = 20;
const int CLAW_SPEED_FAST = 50;
const int CLAW_OPEN = 300;
const int CLAW_CLOSE = 365;
const int CLOSE = -1;
const int OPEN = 1;
const int TURN_CLOSE_THRESHOLD = 10;
const int TURN_DIFF_THRESHOLD = 15;
const int DRIVE_CLOSE_THRESHOLD = 20;
const int DRIVE_DIFF_THRESHOLD = 15;

int convertToMotorSpeed(int proposed) {
	if(proposed == 0) {
		return 0;
	}

	if(proposed < 0) {
		return round( (float)proposed * MOTOR_RANGE / 100 ) - MOTOR_LOW;
	}

	return round( (float)proposed * MOTOR_RANGE / 100 ) + MOTOR_LOW;

}

typedef struct {
	tMotor motorId;
	int encoder;
	int counter;
	int threshold;
} StuckDetector;

void initStuckDetector(StuckDetector* s, tMotor m, int t) {
	s->motorId = m;
	s->encoder = 0;
	s->counter = 0;
	s->threshold = t;
}

void resetStuckDetector(StuckDetector* s) {
	s->counter = 0;
}

bool isStuck(StuckDetector* s) {
	int lastEncoder = s->encoder;
	s->encoder = round(getMotorEncoder(s->motorId));

	if(abs(s->encoder - lastEncoder) < s->threshold) {
		s->counter++;
	}
	else {
		s->counter = 0;
	}

	return (s->counter >= 3);
}

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

bool isCancelled();

bool moveClaw(int target, int dir, int p1, int p2) {
	StuckDetector clawStuck;
	bool canc = false;

	initStuckDetector(&clawStuck, claw, CLAW_STUCK_THRESHOLD);

	target = abs(target);
	p1 = abs(p1);
	p2 = abs(p2);

	setMotorTarget(claw, target * dir, p1);

	while(!getMotorZeroPosition(claw) && !canc) {
		sleep(75);
		canc = isCancelled();
	}

	setMotorSpeed(claw, p2 * dir);

	while(!isStuck(&clawStuck) && !canc) {
		sleep(75);
		canc = isCancelled();
	}

	setMotorSpeed(claw, 0);

	return !canc;
}

#define CLOSE_CLAW() moveClaw(CLAW_CLOSE, CLOSE, CLAW_SPEED_FAST, CLAW_SPEED_SLOW)
#define OPEN_CLAW() moveClaw(CLAW_OPEN, OPEN, CLAW_SPEED_FAST, CLAW_SPEED_SLOW)
