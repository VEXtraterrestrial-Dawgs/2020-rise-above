#pragma systemFile

#define TEAL -1
#define PURPLE 1
const int ARRAY_LENGTH = 5;
const int THRESHOLD = 7;
const int THRESHOLD_ARM = 50;
const int LEFT_RIGHT_THRESHOLD = 3;
const int CLOSE_THRESHOLD = 15;
const int SHORT_INTERVAL = 50;
const int LONG_INTERVAL = 75;
const int MAX_DRIVE_SPEED = 60;
const int MAX_H_DRIVE_SPEED = 75;
const int MAX_DRIVE_ACCEL = 30;
const int MAX_TURN_SPEED = 70;
const int MAX_ARM_SPEED = 90;
const int MAX_ARM_ACCEL = 15;
const float TURN_AVG_KA = 0.8;


// MUST be called inside of a bool function that can return when any operation is cancelled.
#define DRIVE(d) if (driveRobot(d) ==  false) { return false; }
#define TURN(ang, dir) if (turnRobot(ang*dir) == false) { return false; }
#define MOVE_ARM(d) if (moveArm(d) == false) { return false; }
#define CLOSE_CLAW_C() if(moveClaw(CLAW_CLOSE, CLOSE, CLAW_SPEED_FAST, CLAW_SPEED_SLOW) == false) { return false; }
#define OPEN_CLAW_C() if(moveClaw(CLAW_OPEN, OPEN, CLAW_SPEED_FAST, CLAW_SPEED_SLOW) == false) { return false; }
#define WAIT_FOR_MOTOR(nMotorIndex) do { sleep(100); int counter = 0; int lastEncoder = getMotorEncoder(nMotorIndex); while(!getMotorZeroVelocity(nMotorIndex)) { if(isCancelled()) { return false; } if(abs(getMotorEncoder(nMotorIndex) - lastEncoder) < 2) { counter++; }else {counter = 0;} \
	if(counter > 5) { return true; } lastEncoder = getMotorEncoder(nMotorIndex); sleep(15); } }while(false)
