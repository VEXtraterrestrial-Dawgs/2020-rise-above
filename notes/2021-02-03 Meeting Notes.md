# 2021-02-03 Meeting Notes

## Members Present  
Tavas, Athreya, Brad, Sri
  
## Goals  
- Work on the autonomous.
- Discuss changes to the h-drive.
- Discuss strategy and scores in driving.
  
## Driving Notes - Sri and Athreya
- Over the time between our last meeting and now, Sri worked on getting the h-drive to work better, seeing as it not only was important to driving, but it was also important for autonomous. 
- What he decided to do is to change out the motor with a newer, less worn down one, and to get a different omni wheel and replace the old one with that. It ended up working a whole lot better, as the h-drive started to slip a lot less. Also, he realized that the small holes that are in the field affected the h-drive, as it would constantly slip.
- Then, he decided to change up the strategy a bit more. He decided that instead of using up time on finishing the middle area of the purple row, he would only leave one riser in there. He would drive to the teal row, and finish the row and complete a stack in there. That strategy got a total of 108 points, but with more practice, it could be brought up to 140 points. 

## Programming Notes - Tavas

- Today I focused on getting the autonomous program ready so that we could work on tweaking the values for the autonomous strategy.
- I worked on testing and updating the PID coefficients, and looked at ways to make the program more consistent.
- The h-drive and arm drove quite smoothly, and reached the target well, and after some tweaking the drive also drove smoothly.
- However, the turn consistently overshoots its target in one direction, then overcorrects in the other direction, and so on.
- To try to fix this, I tried tuning down the PID control values, but that didn’t work.
- Looking at the gyro value readings, I saw that it jumped around a lot and the gyro sensor wasn’t very consistent even when the robot wasn’t moving.
- Since the turns are based entirely off of the gyro readings, this means that the turn would overreact on noisy readings.
- To help counteract this, I could make a few changes: 
	- **Balance the gyro readings with the encoder readings:** If I took the robot’s estimated position based on the gyro readings and the robot’s estimated position based on the encoder units and combined the results somehow, this could make the turns more accurate. The downside to this would be that I would have to take time figuring out the right balance of how much weight I give to the gyro readings versus the encoder readings.
	- **Read only the encoder units:** If I based the turn entirely on encoder units, it would remove the noise from the readings. The downside to this would be that the wheels can slip, especially with turns, and this could be another source of inaccuracy.
	- **Average out the gyro readings:** While the gyro readings are noisy, they tend to oscillate by 1 or 2 degrees, but mostly stay the same. If I were to calculate the turns using the average of the last *x* gyro readings, it would reduce the noise in the gyro readings. Right now, this seems like the best long-term solution.
	- **Don’t use PID control on the turns:** Since the competition is coming up in less than two weeks, I want to still have time to tweak the autonomous strategy so we can maximize the number of points we get. So, using the built-in movement functions is an option to be considered.
- I will have to think about what solution is right, and by next meeting I will have an updated program I can test out.

## Homework  
- Sri and Athreya will do more driving practice, getting 108 points consistently and maybe getting 140 points a couple of times.  
- Tavas will finalize the autonomous program.

## Plan for Next Meeting  
- The plan for the next meeting is to see the changes that were made to the autonomous made by Sri and Tavas and see the driving scores, and do a dry run for the competition.
