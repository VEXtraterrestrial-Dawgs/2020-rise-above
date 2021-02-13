 # 2021-02-11 Meeting Notes

## Members Present  
Tavas, Athreya, Brad, Sri
  
## Goals  

- To see the progress on both driving and autonomous to see what changes are necessary. 
  
## Driving Notes - Sri and Athreya

Since the last meeting, we have been working on driving. Sri is starting to average 1 minute and 10 seconds in order to score 140 points, which is the target for the competition on Sunday. 

## Programming Notes - Tavas

### Before the Meeting

Since last meeting, I have been working with Sri and Brad to tweak the autonomous strategy and improve driving. In doing so, we noticed a few things:

#### Driver Control - Ghosting 

Even when the driver wasn’t moving the joystick, the robot would slowly drift to the side. To fix this, I increased the ‘dead zone’ in the center of the joystick where the joystick values would be ignored in driverControl.c

#### Autonomous - Turn and Drive Instability

- Though the drives and turns were working well last meeting, they had become increasingly unstable, sometimes infinitely moving back and forth at the end of the move, and sometimes not turning at all.
- In order to fix this, I tried diagnosing the problem, and wanted to test out one theory - if the robot was being moved during the time the gyro was calibrating, the turns would be retrieving inaccurate gyro data.
- This did seem to be the problem, after testing the turns when the robot was calibrated correctly.
- To fix this, I added a feature to the autonomous program that only calibrates the gyro when the touch LED is pressed, so the person running the autonomous program has time to position the robot before the gyro calibrates.
- On second thought, however, the program at the time didn’t actually use the gyro data for turns - it relied on encoder counts instead.
- Upon realizing this, I asked Sri to redownload the program, as the robot likely had an older version of the program installed.
- The new version of the program turned, but still occasionally ended up in an infinite loop of turning back and forth during drives and turns.
- So, I added a feature to the autonomous that reverts to the built-in setMotorTarget function when the robot gets close to its starting position, and this fixed the problem.

#### H-Drive Slipping

- The other problem was that the h-drive continued to slip in the autonomous program.
- To try to mitigate this, I have been raising and lowering the speeds to find the right balance between an h-drive that's slow so it won’t slip, but also fast enough that it can push a riser into a goal.

### During the Meeting

#### H-Drive Slipping

- One of the biggest problems with the autonomous program was that the h-drive slipping made the execution unreliable - sometimes the h-drive wouldn’t slip, and sometimes it would, and this inconsistency meant that a fixed distance to drive forward in order to pick up a riser or h-drive to push something into a goal wouldn’t work a lot of the time.
- To fix this, Sri had an idea: to replace the field tiles where slipping was a larger problem with the goal area field tiles. This helped mitigate the problem.

#### Arm Speed

I also lowered the arm speed because one thing that was happening a lot was the risers being slammed down onto another riser instead of stacked gently, which caused the tower to topple.

#### Turn Offsets
- Another problem that was frequently occurring was that when the h-drive and pushed a riser into a goal, depending on the angle at which the robot approached the riser, at the end of the h-drive move the robot would be slanted one direction instead of perfectly straight.
- This meant that when the robot was slanted, it either turned too much or too little to pick up the risers and stack them.
- To fix this, I added a feature that corrects for the offset in the turn function. The feature works like this:
	- At the end of the turn function, the gyro is reset. So, at the beginning of the next turn, the gyro value should be 0.
	- At the beginning of the turn function, the robot gathers the average of 5 gyro readings, each reading being collected every 25 milliseconds.
	- This results in a brief pause before each turn where the robot is just gathering data, and this makes sure that the gyro offset reading is accurate.
	- Then, when calculating the turn’s encoder target, the offset (which is the gyro average that was collected in the last step) is subtracted from the angle that the robot needs to turn, and the result of that is converted into encoder units.
	- So, for example, it will turn less than 90 degrees if the robot is already slanted to the right, and more than 90 degrees if it starts off slanted to the left.
- This improved the turn accuracy and the autonomous is currently able to get a stack of three risers in the first goal.
	
## Homework  
- Sri and Athreya will practice for 140 points in driving for the competition
- Tavas and Sri will work on the autonomous, aiming for 76 points in driving (one completed stack on both the purple and teal rows)

## Plan for Next Meeting  
- Discuss the plan for the competition and make last-minute adjustments to the program and driving.
