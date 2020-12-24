# 2020-12-23 Meeting Notes

## Members Present  
Tavas, Athreya, Brad, Sri

## Goals  
- Debug why running datalogLeftRightEncoderTest.c returns an error.
- Work towards testing PID coefficients.
- Discuss problems with the robot and driving.

## Driver Notes - Sri and Athreya

- We did our best to get as much driving. 
- While driving, Athreya noticed that the arm was picking up the risers at an angle which was causing problems because it made it hard to stack the third riser on the second riser which is required to make a completed stack. 
- We tried to make small changes to the claw so it wouldn’t pick up anything at an angle but it didn’t help. 
- After today, we decided that we would try and add a beam at the bottom of the claw to make sure that the riser isn’t able to tilt forward.

## Programming Notes - Tavas  

- This week I wanted to focus on testing out and finding PID coefficients. For the autonomous program, the only thing left to have a working program is to find the right coefficients to multiply the proportional, integral, and derivative values inside of PIDControl so I can return a reasonable motor speed that will make the right corrections to the robot’s course.
- To do this, I needed to run the program [datalogLeftRightEncoderTest.c](../code/prototypes/datalogLeftRightEncoderTest.c) so I could get a good idea of how different the two sides of the drivetrain were.
- Then, starting with the PID controller for the left wheels, I would make changes to the proportional coefficient in [autonomousfn.c](../code/autonomousfn.c), look at the graph of error, and see if the proportional corrections were making enough of a difference in correcting the robot’s path. If not, I would increase the proportional coefficient, and when I found a good coefficient, I would move on to altering the integral and derivative coefficients.
- However, since datalogLeftRightEncoderTest was returning an error, I tried to debug it to see how I could make it work.
- Removing the datalogDataGroupStart and end inside of the program worked, with the datalog being filled out. However, This also meant that only one column was being logged per row - so each variable I wanted to log in the same row went into different lines. 
- In discovering that removing the datalogDataGroupStarts and ends were the problem, I had created a program [datalogTestBasic.c](../code/prototypes/datalogTestBasic.c) to see if the problem was that datalogging didn’t work.
- When this program returned an error, I created [basicProgramTest.c](../code/prototypes/basicProgramTest.c) to check if downloading a simple program that just prints something on the brain screen would download without an error.
- Then, when I found that it downloaded correctly, I added a datalog without the datalogDataGroup start and end to see if that would download, and it did.
- To test and find PID coefficients, I will coordinate with Sri to run tests on the robot driving, and log the error, integral, and derivative values inside of the PIDControl function. 
- Then, I will look at the results in an Excel file and change one PID coefficient accordingly, and rerun the tests.

## Homework  
- Tavas and Sri will coordinate on testing PID coefficients
- Sri and Athreya will practice driving and fix the claw so it lifts a riser straight.

## Plan for Next Meeting  
- Discuss changes to the autonomous.
- Discuss changes to the claw.
