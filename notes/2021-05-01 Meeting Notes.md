# 2021-05-01 Meeting Notes

## Members Present
Athreya, Brad, Sri, Tavas

## Goals
- Discuss driving and autonomous progress

## Driving Notes 

- Since last meeting, our highest score in driving has been 102 points, with three completed stacks in the teal row.
- Our goal for the next meeting is to practice consistently scoring all three completed stacks in the teal row, and also one stack in the purple row.

## Programming Notes

- Over the past week, I worked on creating a claw movement function and improving the battery life of the robot.

### Claw Movement

- For the claw, since the claw would need to close different amounts depending on the exact way the claw approached the riser, I decided to use stuck detection to move the claw.
- Essentially, my idea would be to create a “move claw” function that would take in the direction (if the claw was being opened or closed) as well as an encoder value and two speeds.
- The claw would open or close at the faster speed until it reached the encoder value. The idea behind this is to speed up the claw movement until a certain point where the claw needs to slow down to approach its target.
- The claw will slow down and when it detects that it can’t move any further, it will stop.
- I implemented this and also created basic OPEN_CLAW and CLOSE_CLAW macros to call the move claw function with certain parameters.

### Battery Life

- One of the problems we were having with our robot was that the battery drained too quickly, so I thought of ways that the battery is maybe overused.
- One thing that I thought of was the claw in driver control. 
- The claw would close, and grip the risers tightly, needing constant energy to keep the claws in place.
- Although this couldn’t be removed completely, I could stop moving the motors when the claw was stuck.
- I implemented this, and it didn’t make a very large change in battery life. I will continue to brainstorm and implement ways to improve the battery life.

## Meeting Plan

### Homework
- Sri and Athreya will practice driving.
- Brad will give feedback on driving strategy.
- Tavas will finish the autonomous functions by next Wednesday.

### Plan for Next Meeting
- Discuss progress in driving, strategy, and autonomous.
