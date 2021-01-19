# 2021-01-18 Meeting Notes

## Members Present  
Tavas, Athreya, Brad, Sri

## Goals  
- Discuss the competition and plans to move forward.

## Driving Notes

### Competition Scores:

| Run Number | Driver | Score |
| --- | --- | --- |
| 1 | Sri | 39 |
| 2 | Athreya | 70 |
| 3 | Athreya | 70 |

### Problems and Improvements Needed

- During the driving runs, we noticed a few things that caused us to get just the two completed stacks in one minute:
	- With the two arms, a lot of time was wasted aligning the top arm with the riser, lifting it, moving forward, raising the bottom arm, and spending time lowering both arms separately. Because the arms functioned as separate units, controlling them took a long time and this resulted in a driving run that was mostly lifting and stacking risers, and not much moving around.
	- With our robot being very fast in general (drive and h-drive), we spent a lot of time overshooting the target and having to make small adjustments to align with the risers. This took up a lot of our time, and also resulted in most of the driving run being dealing with risers. This was true especially for turning.
	- Again, because of our speed-optimized base, our base was weak when it came to pushing risers around, especially with h-driving. This caused h-driving a riser into a goal to take an extra second.
- So, what we were finding was that the robot was very difficult to control.

### Plan for the Future

- To be able to maximize our score, we need to have no wasted motion and no wasted time. Teams that did well were very quick to lift risers and move around, and multitasked a lot.
- One team during the competition that stood out had a robot able to glide around the field. They did this through using the h-drive and the drive at the same time, and this made the robot very fast, efficient, and controllable. We can try out doing the same thing with our robot, and see if that helps with driving.
- With a lot of time being wasted in turns, we can test out if tank turns would work better than arcade style turning. Since a lot of teams have very minimal turning, and only ever use two or three angles (facing straight forward and facing 90 degrees to one side) and use the h-drive for the rest, tank turning might be easier to control. The downside would be that driving and turning can’t happen at the same time, but arcade turning versus tank turning is something worth testing out.
- With a lot of time wasted in lifting risers, we need to move towards a single-arm design. Most teams we saw had just one arm that gripped one or two risers at a time, so we will come up with a design for this and start building a single arm for lifting the risers.
- The h-drive is another problem - it keeps slipping and not moving the robot at all. At first, we can try the simplest solution, wiping down the field and the h-drive wheel to see if it’s just dust and debris causing the h-drive problem. If that doesn’t work, we can also try replacing the wheel, lowering the gear ratio, or lowering the speed or acceleration of the h-drive.

## Programming Notes

### Problems and Improvements Needed

| Run Number | Score |
| --- | --- |
| 1 | 0 |
| 2 | 1 |
| 3 | 0 |

- A few things we noticed about the low scoring autonomous:
	- With the recent hardware fix just a few days before the competition, we didn’t have time to tweak the autonomous program values enough to get the program working.
	- The h-drive also played a factor in the low autonomous score - the wheel slipping caused the autonomous strategy to get messed up early on.
	- There was no backup plan: in all the runs, the robot would get stuck and then we weren’t able to reset the robot to its starting position and score more points.
	- The drive, turn, and h-drive movements never were very accurate, with the removal of the PID control to keep the robot.
	- In tweaking the autonomous values, there was a typo very early on in the autonomous sequence, and that caused the robot to just h-drive and move forward and then stop.

### Plan for the Future

- We need to fix the h-drive slipping so that the h-drive movements will work reliably.
- We need to reinstate the PID control so that our movements are more accurate.
- We can also mirror the autonomous sequence so that if the autonomous program gets stuck, we can move the robot to the other side starting position and run the mirrored version of the autonomous, which will score more points. This will not just be a backup, but also a program to run if we get done with one side early.

## Efficiency and Workflow Improvements

- One of the things we need to work on is efficiency and communication as a team. To help improve this, we compiled a list of actions we can take.
	- We need to be faster at downloading and debugging programs, and this is a skill everyone on the team should have.
	- We need to be faster at making build and design changes to the robot so we have more time to practice driving and change programs as needed.
	- We need to be better at noticing design problems earlier and fixing them earlier so we have time to prepare with the new changes.
	- During meetings, the person with the robot should always have their camera on or be sharing their screen so that everyone in the meeting can see what is happening and communication can be improved.
	- We should be transferring the robot around or have team members go to the robot so everyone can do what they need to do with the robot for their role.
	- We can also divide roles on the team better, so even though we have drivers, builders, and programmers, we can also have certain members tweak values for autonomous and give feedback on driving, and so on so we can be more efficient at getting these things done.
	- When working on something, it should be functional when done working on it. This includes always pushing clean code to GitHub and never pushing any code that doesn’t compile. It also means that when working on building, the robot should always be functional between building sessions so that driving and autonomous work can still occur.

## Homework  
- Sri and Athreya will work on testing running the h-drive and drive at the same time to glide, testing arcade turning versus tank turning, and work on making changes to the driving strategy.
- Brad will work on building a new arm for the robot and mounting it to the base.
- Tavas will work on reinstating PID control, creating a program for tank turning, and changing the driver control program based on Sri and Athreya’s feedback about tank turning versus arcade turning.

## Plan for Next Meeting  
Discuss changes in programming, driving, and building.
