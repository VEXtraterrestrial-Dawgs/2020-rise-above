# 2021-01-06 Meeting Notes

## Members Present  
Tavas, Athreya, Brad, Sri

## Goals  
- Practice driving
- Work on autonomous

## Driver Notes - Sri and Athreya

#### Driving Scores

- Below are the driving scores for the past week.

![Driving Scores](../data/2020-12-30--2020-01-06-driving-scores.png)

#### Progress and Today’s Meeting

- I continued to practice, hitting over 75 a couple of times, but sometimes make one mistake, causing me to score 40 or under 40. Apart from that, I didn’t do anything. 

## Programming Notes - Tavas

### PID Testing

Since last meeting, I worked on debugging the h-drive and turn functions and finishing tuning the coefficients for the arms and the h-drive.

### Work Since Last Meeting

#### Debugging the H-Drive

- Fixing the h-drive was fairly simple, I looked through the h-drive function and found that the encoderTarget calculation was wrong. 
- Instead of multiplying the encoder units per rotation after dividing the distance by the wheel circumference and the gear ratio, I had to multiply encoder units per rotation first so that the integer division would not round to zero.

#### Debugging the Turn

- After printing out the gyro value and error on the brain, I still couldn’t figure out why the turn was not working. The error values were reaching over 400 encoder units, which didn’t make sense given that I was trying to make only a 90 degree turn (which is about 100 encoder units).
- The gyro value being printed to the brain seemed accurate as well, so I was not sure what was wrong.
- To simplify and clean up the code, I added an angleToEncoderUnits function that returned the encoder units given an angle so I could reduce duplicate code and also find the error in degrees first and then convert that to encoder units, which would make it easier to debug.
- Since the turn still goes infinitely, I will continue trying to debug and fix this problem.

#### Coefficient Testing - H-Drive

- After fixing the h-drive, I ran tests by just running the h-drive, looking into the graph of error in the robotc debugger console, and updating the PID coefficients accordingly.
- I achieved an error graph that looks like this.

![Graph](../data/2021-01-03-h-drive-error.png)

- The h-drive went fairly smoothly, and consistently didn’t overshoot its target.

#### Coefficient Testing - Arms

- With the same testing procedure as I did with the h-drive, I tuned the PID coefficients for the arms (both arms use the same coefficients) and achieved the error graphs below.

![Top Arm Graph](../data/2020-01-03-top-arm-error.png)

![Bottom Arm Graph](../data/2020-01-03-bottom-arm-error.png)

- The top arm error smoothly reached zero, and didn’t overshoot its target.
- However, while the bottom arm also reached an error of zero eventually, it wobbled for a bit after reaching its target. This is probably because since the bottom arm was shorter than the top arm, the derivative coefficient was slightly too high.
- Since the bottom arm doesn’t always wobble at the end of its movement and it doesn’t wobble very much, I will leave it for now, but if this becomes a problem in the future, I will have different PID coefficients for each arm.

### Today’s Meeting Work

- During the meeting today, I focused on tuning the coefficients for the drive function.
- Because the driving was very far off, and constantly swerved around, I didn’t use the datalog.
- Instead, I opened up the datalog spreadsheet from last time and using the error, integral and derivative values logged, I put the values into a calculator and adjusted the coefficients so that it would give a reasonable speed for the error.
- After that, I watched how the robot performed and adjusted the coefficients by eye.
- After the changes, the robot drives relatively smoothly and stops when it is supposed to. To refine the coefficients for a more precise stop, I will use the datalog.

## Homework  
- Sri and Tavas will coordinate to run tests for autonomous
- Sri and Athreya will practice driving

## Plan for Next Meeting  
- Discuss progress in driving, design, and autonomous.

