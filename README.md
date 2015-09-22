# Robot-Steel
This contains the source code of the PNBHS 2903 2015 Robot Steel. The source code is based on a task system and will recieve many revisional updates throughout the season. 


The Tasks and functions are as follows:

task: initalizeflywheel resets the input values to the defaults set below.

task: flywheelRamp runs InitializeFlywheel then ramps flywheel ramp up with defined variable.

Uses inbuilt while(true) loop, so dont used in while(true) loops unless triggered :P.

task: flyWheelTerminate sets the value of motors to 0, does not disable.

task: TankDriveMovement is a tank running during intervals for controlling the tank drive system.

void: RunFeed runs the feed on the robot.


And the button mappings are:

Btn5U: Startup ramp switch.

Btn5D: Stop flywheelRamp/flywheel.

Btn8U && Btn8D: Toggle flywheel Speed up and down.

Btn8R: Increase Right flywheel motor speed by 5 and decrease left by 5.

Btn8L: Decrease Right flywheel motor speed by 5 and increase left by 5.

Btn6U: Feed Forward (actual reverse).

Btn6D: Feed Backwards (actual forward).

#CPS Chart
![Alt text](http://i.imgur.com/4m4KZDI.png)
