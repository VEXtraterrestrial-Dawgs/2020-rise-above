# 2020-12-19 Meeting Notes

## Members Present  
Tavas, Athreya, Brad, Sri

## Goals  
- Fix the squealing problem the robot has
- Measure the width of the drivetrain for programming
- Run the test program datalogLeftRightEncoderTest.c for programming

## Driver Notes - Sri
During the time between meetings I have been working on my driving, and am sometimes able to hit upwards of 90 by a tiny bit. I can consistently and easily get the first triple riser stack, do decent on the middle, but don't do that well on the last stack. 

## Programming Notes - Tavas

Today I wanted to test a few things for the programming work I have been doing over the past week. 
- One of the key features of the program I am working on is that all the functions to drive the robot and raise the am all take in inputs in millimeters. 
- This makes the main autonomous program much easier to write because all the measurements can be made beforehand. 
- In the turnRobot function, to be able to internally calculate how far in encoder units each wheel needs to travel given the angle, I need to know the distance between the wheels, which acts as the radius of the turn. 
- I also need to know the location of the color sensor so I know which part of the robot is passing over a black line when the color sensor finds one.

Here are those measurements:

| Value Being Measured | Length in Millimeters |
| --- | --- |
| Distance Between Wheels | 132 |
| Width of Robot | 215 |
| Length of Robot | 291 |
| Color Sensor to Front of Robot | 16 |
| Color Sensor to Back | 275 |
| Color Sensor to Left | 106 |
| Color Sensor to Right | 107 |

- I also wanted to run a test to find out if one of the drivetrain motors is weaker than the other, and if so, by how much.
- This will help me make autonomous driving a lot more accurate because I will be able to give more power to the weaker motor so it balances out and always drives in a straight line.
- In this test, I will also read the gyro sensor readings to gauge the accuracy of the gyro sensor as well.
- To test this, I had created a program: [datalogLeftRightEncoderTest.c](../code/prototypes/datalogLeftRightEncoderTest.c). This program drives both wheels at the same speed for a predetermined distance and logs the encoder values and gyro reading.
- However, we ran into an error where the brain would say “Invalid program data”. So, I will look into why this is happening and we will run this test next meeting or sometime in between.

## Homework  
- Sri will practice driving and measure out each of the steps in the autonomous strategy so we can put that into the main autonomous program.
- Tavas will fix the testing program and work more on the autonomous program.

## Plan for Next Meeting  
- Run the programming test.
- Review the work everyone has done since this meeting.
