# 2020-10-31 In Person Meeting Notes

## Members Present  
Athreya, Brad, Sri, Tavas

## Goals  
- Discuss the changes Brad made to the robot.
- Test the two robots we have so we can figure out how to merge them.
- Work on brainstorming solutions to the problems we have with our robot.

## Build Progress - Brad

**Brain Position and Front Beams**  
- Today I worked on fixing the problems for the brain and the front two beams, as well as reduced the drifting by a bit. 
- I also increased the overall stability of the robot in the front two beams that help the robot move with a riser. 
- The things that I did in order for the brain to be able to be accessed was that I tried out a lot of positions for the brain to be, such as mounting it sideways on the back, but then I realized the easiest and most effective thing to do was to flip the brain backwards in the same position it was in. 
- In order to find the right position for the beams to be was that I had to change it by 1 “dot” length of a beam every single time, and then I would try to make sure the beams could cover the length of the riser so that it could turn with it. 
- This would be important because even though the side beams won’t be the main thing keeping the riser in place in the front later on, it’s still good to have it work as a support, as the front arm would hold the riser later on.

**Robot Drifting**  
- To figure out how to fix the robot drifting was hard. 
- First, I looked at the general design of the robot, and didn’t really find anything useful. 
- What I did after that was I took apart all of the wheels and their axles to see if I did anything wrong while building them. 
- What I did find was that 1 of the axles wasn’t in the middle dot of a beam, it was actually at the top, unlike all the other axles. 
- This inaccuracy definitely played a huge part in the drifting of the robot. 
- Even though the robot still drifts a bit, I think later on when more weight such as the back arm is put, the drifting will diminish.

**Stability of Front Beams**  
- I also noticed that the two front beams were very wobbly and not supported well. 
- What I did was that I incorporated something from Tavas’s first design, which was that I used 2 1x18s and then I used it to match the length of the old 2x20 beams by putting less of the beam to the side of the robot rather than in front of. 
- Then, I connected the 2 1x18s with worm brackets and standoffs. This really did help a lot with the stability of the two front beams.

**Problems to Solve for Next Time**  
- I did find a problem with the robot which was that it was squealing when it moved. 
- That meant that there was something on the robot that didn’t have an washer, so there was a moving part next to a stable part, such as a rubber stopper right next to a beam. I will look into that and try to find the solution to that.
- I also found another problem that was that the robot was moving to the side. However, I think that this is more of a problem that the power of the motors on the drivetrain are not the same, which would result in the robot moving more to one side than the other.

## Programming Progress - Tavas

**Position Tracking Progress**  
- Over the last week I have been continuing to work on finding the right algorithm to track the position of the robot.
- I have made progress on this and now I know how to find the change in x and y position of the robot, but one problem I have run into is that if the robot starts at an angle that is not parallel to the x axis, the measure of distance x and distance y will be x and y relative to the robot, not relative to the field, which is what I want.
- I will work on finding a solution to this problem next week.

**Turning**  
- During the meeting today, I noticed that turns on the robot were unusually slow, especially on Brad’s robot and couldn’t turn with a riser.
- To try to find out what the problem was exactly, I observed the robot as it turned.
- I found that even when the joystick on the controller was moved to the far left, the inner wheels of the turn weren’t moving and only the outer wheels were. 
- While this is a valid way of turning, this means that there isn’t as much force in the turn and the type of turns we were trying to make when turning with a riser were sharp turns and pivots, where the position of the center of the robot doesn’t change.
- To do this, I would have to turn the inner wheels of the turn backwards by the same amount as the outer wheels of the turn, and so this is something that I should change with the current turning algorithm in the program.

**Driver Control Program**  
- During the meeting today, I also created a driver control program for the robot we had built with the h-drive and the two single beams connected parallel to the ground instead of a traditional base. This is called [driverControlRainbow.c](../code/prototypes/driverControlRainbow.c) (because the robot has a lot of colored pieces) in the Prototypes folder in the Code folder.

## Driving Progress - Sri

- Today I began driving with Tavas’s wheelbase, and I thought about the pros and cons of it. 
- A really great feature of it is the h-drive mechanism. It leaves room for horizontal error, because it can move horizontally. Thus, it makes aligning with risers a lot easier. 
- Another feature that I, as a driver, like is the fact that it does not drift. 
- This makes turning much easier as it does not drift and therefore makes everything more accurate. 
- A thing that I do not like about it is the speed of it (I think that the base is too fast to be easily maneuverable), but that can be changed either using gear ratios, or just by tweaking it in the programming. Either way, it isn’t really that huge of a problem. 
- During the meeting today, we tested to see which wheelbase was more accurate. 
- To do this, we devised a test where Athreya and I would take turns driving both robots (three times each) and trying to drive the robot four black squares on the field forward and one square to the left. This tested how easy it is to turn and move forward accurately.
- I noticed a flaw in my driving that might make something major happen. I always turn a little bit too much to the left. I will just need to practice more. 
- As of which wheelbase is easier to maneuver, I think the one with the h-drive is better because it is more accurate.

## Design/Strategy Progress - Athreya

- Adding on to my last strategy, Brad’s current plan for the arm of the robot is going to be a four bar lift so it will be a little bulky and you can't make any sharp turns. 
- However, this also means that you can lift things like two risers stacked on top of each other. So, we have a solution to a big problem. 
- This means that I should go for the right/left risers depending on starting position. 
- I decided on this because there will be enough room to turn and pick up the riser. 
- There will be a stack with two risers that I will pick up and stack on top of the cube I pushed into the score zone. 
- From then on I would easily push the risers into their scoring positions.

## Homework  
- Brad will continue working on the robot. He will merge the two robots we have based on each robot’s advantages and disadvantages. He will also work on fixing some of the problems we discussed today.
- Tavas will work on fixing the problems in the program we have with turning. He will also continue working on developing the algorithm to track the robot’s position on the field.

## Plan for Next Meeting  
On Wednesday, we will have a remote meeting to discuss the work everyone did over the past week as well as make decisions on any adjustments we need to make to the robot.

