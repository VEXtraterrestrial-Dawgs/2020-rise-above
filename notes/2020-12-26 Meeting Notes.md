# 2020-12-26 Meeting Notes

## Members Present  
Tavas, Athreya, Brad, Sri

## Goals  
- Run through the driving strategy in 60 seconds and see how many points we get.
- Discuss changes to the claw and make additional changes if necessary.
- Run tests for autonomous.

## Design Notes - Brad, Sri, Athreya

- We added a plate on the bottom of the claws to try to help keep the riser straight.
- Though this succeeded in keeping the riser straight, it also made it harder to lift and grip a riser, so we took off the plates.

## Programming Notes - Tavas  	

Over the past week I worked on cleaning up the autonomous program and compiling the data that Sri sent me.

### Work I did Since Last Meeting

**Program Changes:**

In the program, I made a few changes.
- I added a ‘clip’ function that is called after PIDControl, and the clip function makes sure the motor speed isn’t more than a maximum speed limit, and that it hasn’t increased by more than the maximum acceleration since the last time the motor speed was changed.
- I also combined the moveTopArm and moveBottomArm function into one moveArm function that takes the arm motor to run the function on as a parameter. This helped remove duplicate code.
- Also, in the turnRobot function, instead of calculating the current position of the robot by retrieving the motor encoder, I took the gyro reading and converted it into encoder units. This was because in the testing, the gyro sensor was remarkably stable and seemed accurate.
- Finally, I initialized all the Kp and Ka values for all the PID controllers.

**Testing:**

Since last meeting, Sri ran the test datalogLeftRightEncoderTest.c so we could test out if one drivetrain motor was weaker than the other and how accurate the gyro sensor was. The graphs of the gyro readings and encoder readings are displayed below, and the data can be found at: [2020-12-26-datalogLeftRight.xlsx](../data/2020-12-26-datalogLeftRight.xlsx).

![Graphs](../data/2020-12-26-datalogLeftRight-graphs.png)

**Encoders**
- The result was that the difference between the left and right sides of the drivetrain was initially very noisy, with the left side initially being 10 encoder units more than the right, then the right side being 10 more than the left, and so on. Eventually, the readings stabilized around the right side being around 3 encoder units ahead of the left. 
- This made me determine that I will need a large Kp for the right side of the drivetrain (which deals with correcting the difference between the left and right sides of the drivetrain). If the errors are usually around 3 encoder units but sometimes reach as large as 10 encoder units, I would want to apply a power difference of about the same amount as the error.

**Gyro**
- The result was that the gyro sensor reading was relatively stable around -6 degrees.
- This made me determine that using the gyro sensor instead of encoder units to determine current location for turnRobot would be more accurate.

### Today’s Meeting

Today, I wanted to test out autonomous.c to confirm that the driveRobot and turnRobot functions were working correctly and to collect data so I could tune the Kp coefficients. The graphs are displayed below, and the data can be found at:
[2020-12-26-firstPid.xlsx](../data/2020-12-26-firstPid.xlsx)

![Graphs](../data/2020-12-26-firstPid-graphs.png)

**Results**
- The error for the right side of the drivetrain was consistently above 0, indicating that the Kp was too low. The error for the left side reached to below 0 at the end, indicating the robot overshot the target and the Kp should be reduced.
- The integral graph for the right side of the drivetrain again showed that Kp should be increased, because ideally the corrections being made would help the integral value balance out to 0 over time.
- The derivative graph, which stayed at 0 the entire time for both controllers, indicated that something was broken with the derivative calculation. Looking closer at the code, I saw that since the derivative was being calculated by dividing integers, the quotient would be floored to 0. I fixed this by converting the error to a float, and will rerun this test to determine whether the fix worked.
- The gyro graph showed that the robot didn’t drive in a straight line, and this is reinforced by the fact that the error over time for the right controller was mostly above 0, indicating that one side of the robot was ahead of the other.


## Homework  
- Everyone will think of ways to help with the angle problem that the arm was facing.
- Sri will practice driving. 
- Sri will fix the problem with the axle on the bottom arm slipping out of the motor.
- Tavas will fix the problems with the autonomous program.
- Tavas and Sri will coordinate to continue running tests.

## Plan for Next Meeting  
- Discuss progress in driving, design, and autonomous.

