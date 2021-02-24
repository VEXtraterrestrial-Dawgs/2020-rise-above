# 2021-02-23 Meeting Notes

## Members Present  
Tavas
  
## Goals  
- Work on tweaking the autonomous program.

## Build Notes - Tavas

- The one thing I changed today was that I made the hooks closer together because I noticed the risers were hard to pick up reliably in autonomous. Now, the risers are much easier to lift. 

## Autonomous Notes - Tavas

- While running the autonomous, I noticed that the drives, turns, and arm raises were all starting their movement and then immediately going to the next step, so the arm would raise while the h-drive was moving, etc.
- I noticed that this was caused by the change I had made yesterday to fix the LED cancelling — before this fix you could only pause the program by hitting the touch LED if the h-drive was moved.
- To fix this, instead of using waitUntilMotorStop to ensure that each movement had enough time to complete, I used a while loop that constantly checked if the motor had stopped and if the LED was pressed. If so, it would return false from the function.
- The problem was that this while loop didn’t work. The conditions were never true, and using different functions to check if the move was complete didn’t help.
- To fix this problem, I defined a macro called WAIT_FOR_MOTOR. This macro was essentially the same as the waitUntilMotorStop function, except for one difference: it was also constantly checking to see if the LED was pressed to cancel the program.
- With this fix, the robot movements started working again, and the LED cancelling still worked. 
- After that, I adjusted the autonomous program values, and this is when I noticed that the risers were hard to pick up and I made the change shown in the Build Notes above. 
- Currently, the robot can get 1 riser in each goal in both the teal and purple rows. It is not yet able to get the completed stacks yet because it drives forward too fast and knocks over the riser stack. 
- This is a simple fix, and I plan on fixing this next time I work on the autonomous tuning.

## Homework  
- Sri and Athreya will practice driving.
- Tavas will tweak the autonomous program.

## Plan for Next Meeting  
- Discuss driving and autonomous progress.

