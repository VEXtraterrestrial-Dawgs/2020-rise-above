# 2021-02-06 Meeting Notes

## Members Present  
Tavas, Athreya, Brad, Sri
  
## Goals  
- Fix the functions in the autonomous
- See what progress Sri made on the driving
- Discuss strategy and scores in driving.
  
## Driving Notes - Sri and Athreya
- Over the time in between last meeting and now, Sri worked on driving even more, and attempted to get to 140 points. Not much else happened, except for the h-drive starting to act up again. 

## Programming Notes - Tavas

### Before the Meeting

- Since last meeting, I have been working on finalizing the turnRobot function.
- Last meeting, I came up with the main problem the function was facing: gyro readings were noisy and tended to jump around a lot, causing the robot to also overreact to the gyro readings.
- I also came up with a list of solutions, as seen in [2021-02-03 Meeting Notes](2021-02-03%20Meeting%20Notes.md).
- I started trying to implement the third solution, averaging out the gyro readings. In the turnRobot function, I added a variable, gyroAvg, which was the accumulated value of the gyro readings.
- Each iteration of the while loop, gyroAvg was multiplied by a certain coefficient (so that over time previous gyro readings would be a very small percentage of the gyroAvg variable) and added to the new gyro sensor reading multiplied by 1 minus the coefficient.
- This, I hoped, would stabilize out the gyro values and remove the noise.

### During the Meeting

#### Autonomous - Turning

- When testing out the turn function, I found that though the turn was much cleaner and didn’t oscillate back and forth a lot, the turn consistently overshot its target.
- With the competition coming in a week, I wanted to finalize all the robot movement functions by the end of the meeting today, so I resorted to the second solution instead: using encoder units instead of gyro readings for the turns.
- This helped a lot, and now the robot turns more or less accurately.

#### Autonomous - H Drive

- Although we changed out the motor and wheel of the h-drive a few meetings ago, and it seemed to be working well, the h-drive started slipping again recently.
- To find a solution, I looked at the movement of the h-drive as it drove.
- During the runs where the h-drive slipped a lot, the wheel was moving very fast, but the times it didn’t slip, it moved a lot slower.
- So, I just lowered the max speed of the h-drive in the autonomous program, and the h-drive started to be more consistent.

#### Autonomous - Arm

- The problem with the arm movement was that it was starting to bounce a lot when it reached its target, consistently overshooting in one direction and then overshooting in the other.
- To save time, I decided to remove PID control from the arm altogether, and use setMotorTarget instead. This worked, and the arm moves accurately now.

#### Driver Control

- In watching Sri’s driving runs, I noticed a few things:
	- A lot of time was spent overshooting a target and then trying to readjust.
	- The h-drive was slipping way too much, which also wasted time.
- To fix both of these problems, I just lowered the speed of the h-drive and lowered the speed of the drive, and it works better now.

#### Plan For Next Meeting

- Now that the autonomous movement functions are completed, the autonomous strategy has to be tweaked - we need to measure and tweak the distances of each drive, turn, h-drive, and arm move.
	
## Homework  
- Sri and Athreya will do more driving practice, aiming for a consistent 140 points.
- Someone will work on tweaking the autonomous strategy

## Plan for Next Meeting  
- Discuss progress made in driving and autonomous.
