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
const int ARM_HIGH = 2000;
const int ARM_STACK = 1770;
const int ARM_LIFT = 270;
const float ENC_UNITS_PER_MM = (float)ENCODER_UNITS_PER_ROTATION / ((float)WHEEL_CIRCUMFERENCE * DRIVE_GEAR_RATIO);
const float ENC_UNITS_PER_DEGREE = 0.5 * DRIVETRAIN_WIDTH * degreesToRadians(1) * ENC_UNITS_PER_MM;
