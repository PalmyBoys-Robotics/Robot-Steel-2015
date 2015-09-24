#pragma config(Sensor, dgtl1,  rightEncoder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  leftEncoder,    sensorQuadEncoder)
#pragma config(Motor,  port1,           rightd,        tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           leftd,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           leftf1,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           leftf2,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           rightf1,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           rightf2,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           feed,          tmotorVex393_MC29, openLoop)

#pragma platform(VEX)
#pragma competitionControl(Competition)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

//define motor
#define vexMotorOff 0
#define vexMotorMax 127
#define vexMotorMin -127

//flywheel automatic speed control
bool manualFlyWheelSpeedControl = false;

//usage in switch statement for intake/feed
int IntakeControl;

//Flywheel Definitions
//defined in initalizefLyWheel Task

int speedVariableR;
int speedVariableL;

int MaxSpeedR = 70;
int MaxSpeedL = 70;

#define StartSpeedR 0
#define StartSpeedL 0

// 1 will take 30 * 127 (maxspeed / IncremntRate)
// = 3800ish milisecs
//i.e. set higher to ramp up faster!
const int IncremntRateR = 5;
const int IncremntRateL = 5;

//bool flywheelRampActiveR;
//bool flywheelRampActiveL;

//int used in ToggleFlywheelSpeed up & down as simple way of creating a toggle button
int kNumbOfAttempts;

// @about returns true if a value is above 127
// @from NifftyFucntionLib
// @extraInfo We are only using variables in range 0-127
bool AboveVexMotorMax(int intIndex)
{
	bool AboveVexMotorMin = false;
	if(intIndex > vexMotorMax)
	{
		AboveVexMotorMin = true;
	}
	return (AboveVexMotorMin);
}

//Reset flywheel variables
void InitializeFlywheelVariables()
{
	//reset to automatic mode
	manualFlyWheelSpeedControl = false;

	//The Starting speed of the flywheels
	speedVariableR = StartSpeedR;
	speedVariableL = StartSpeedL;

	kNumbOfAttempts = 0;
}

//Flywheel Definitions End

// @about Flywheel ramp from startSpeed to MaxSpeed
// @from written for Steel originally, but transfered to NifftyFlywheelLib.c
task flywheelRamp
{

	//makes sure all flywheel values are default before starting
	InitializeFlywheelVariables();

	while(true)
	{

		//incremnt motor speed
		if(speedVariableR < MaxSpeedR && manualFlyWheelSpeedControl == false) { speedVariableR = speedVariableR + IncremntRateR; }
		if(speedVariableL < MaxSpeedL && manualFlyWheelSpeedControl == false) { speedVariableL = speedVariableL + IncremntRateL; }

		//Right Side Flywheel
		motor[rightf1] = speedVariableR;
		motor[rightf2] = speedVariableR;

		//Left Side FLywheel
		motor[leftf2] = speedVariableL;
		motor[leftf1] = speedVariableL;

		if(AboveVexMotorMax(speedVariableR) == true) { speedVariableR = 127; writeDebugStream("Motor Flywheel int = above vexMotorMax, *reset to 127"); }
		if(AboveVexMotorMax(speedVariableL) == true) { speedVariableL = 127; writeDebugStream("Motor Flywheel int = above vexMotorMax, *reset to 127"); }

		// @ToD0 Figure out a better way of doing this :P
		// @Warning only works on first run, no way to set it to false if task end
		// if(speedVariableR == 50) { flywheelRampActiveR = true; }
		// if(speedVariableL == 50) { flywheelRampActiveL = true; }

		//dont hog cpu
		wait1Msec(75);
	}
}

// @about used to terminate flywheel
// @deprecated function, kept incase needed
//void flyWheelTerminate()
//{
//	motor[rightf2] = vexMotorOff;
//	motor[rightf1] = vexMotorOff;
//	motor[leftf2] = vexMotorOff;
//	motor[leftf1] = vexMotorOff;
//}

// @about task to run in background that termiantes the flywheel!

task FlyTerminateTerminateSwitch
{
  while(true)
  {
    if(vexRT[Btn5D] == true)
    {
      motor[leftf1] = 0;
      motor[leftf2] = 0;
      motor[rightf1] = 0;
      motor[rightf2] = 0;
    }
    
  wait1Msec(50);
  }
}

// @about Driver control TankDrive task
task TankDriveMovement
{
	while(true)
	{
		motor[leftd] = vexRT[Ch3];
		motor[rightd] = vexRT[Ch2];
		wait1Msec(20);
	}
}

// @about Driver control TankDrive task
task ArcadeDriveMovement
{
	while(true)
	{
		motor[leftd] = (vexRT[Ch2] + vexRT[Ch1])/2;
		motor[rightd] = (vexRT[Ch2] + vexRT[Ch1])/2;
		wait1Msec(20);
	}
}

// @about Runs Feed ast specified speed
void RunFeed(int speed)
{
	motor[feed] = -speed;
}

/////////////////////////////////////////////////////////////////////////
/* Autonomous Routines/Function Definition Area:											 */
/*																																		 */
/* The Code Below is designed for easy creation of autonomous routines */
/* It could be considered a Library for autonomous functions					 */
/* Alot of the code is tanken from my libraries, or from heavily			 */
/* modified versions of code found online.														 */
/////////////////////////////////////////////////////////////////////////


// @about Code to turn robot
// @from my own code on a patch for the Vex Robotics Lakeside 2015 file
// @url: https://github.com/EvolvedAwesome/Vex_Code/commit/35411244fc59402c16f9c5a3da01d869edb6a740
void moveTurn(int power, int time, string direction) //-1 for left, 1 for right
{

		int directionNUMB;
		if(direction == "Left") { directionNUMB = -1; }
		if(direction == "Right") { directionNUMB = 1; }

		motor[leftd] = (power*directionNUMB);
		motor[rightd] = -(power*directionNUMB);
		wait1Msec(time);
}

// @about move robot forward
void moveForward(int speed, int time)
{
	motor[rightd] = speed;
	motor[leftd] = speed;
	wait1Msec(time);
}

// @about Autonomous Move motor with time control
// @from NifftyFunctionLib
void moveMotor(int index, int speed, int time1Msec)
{
	motor[index] = speed;
	wait1Msec(time1Msec);

	string debugstreamMotor;
	sprintf(debugstreamMotor,"Motor: ", index, "Went:", speed, "speed for: ", time1Msec, "Msecs",); //Build the value to be displayed
	writeDebugStreamLine(debugstreamMotor);
}

// @about stop motor from moving
// @from nifftyFunctionLib
void StopMotor(int index)
{
	motor[index] = vexMotorOff;
}


/////////////////////////////////////////////////////////////////////////
/* Competition Area:																									 */
/*																																		 */
/* This is the area for creating you autonomous	routines using the		 */
/* functions above. As well as writing driver control code.		 				 */
/////////////////////////////////////////////////////////////////////////

	void pre_auton()
	{

		if(nImmediateBatteryLevel > 6800)
		{
			MaxSpeedL = 67;
			MaxSpeedR = 67;
		}
		else if(nImmediateBatteryLevel < 6800 && nImmediateBatteryLevel > 6400)
		{
			MaxSpeedL = 80;
			MaxSpeedR = 80;
		}
		else if(nImmediateBatteryLevel < 6400)
		{
			MaxSpeedL = 100;
			MaxSpeedR = 100;
		}

		bStopTasksBetweenModes = true;
	}
	///////////////////////////////////////////////////////////////////
	task autonomous()
	{
		startTask(flywheelRamp);

		while(true)
		{
			//if the motors are at full speed, start the feed.
			if(speedVariableL <= 80 && speedVariableR <= 80)
			{
				wait1Msec(3000);
				RunFeed(127);
			}

			//dont hog CPU
			wait1Msec(30);
		}
	}

/////////////////////////////////////////////////////////////////////////////////////
task usercontrol()
	{

		startTask(TankDriveMovement);

		while (true)
		{

			bool DriveToggleButtonPressed = false;
			//if you want arcade use 7U, YAY :)
			if((vexRT(btn7U) == 1) && DriveToggleButtonPressed == false) { stopTask(TankDriveMovement); startTask(ArcadeDriveMovement); DriveToggleButtonPressed = true; }
			if((vexRT(btn7U) == 1) && DriveToggleButtonPressed == true) { stopTask(ArcadeDriveMovement); startTask(TankDriveMovement); DriveToggleButtonPressed = false; }

			//Start the ramp up flywheel
			if(vexRT[Btn5U] == 1) { startTask(flywheelRamp, 100); }

			startTask(FlyTerminateTerminateSwitch);
			
			//// @about Simple intake
			//// @extraInfo uses a state machine.
			//Btn 6U is Forward			//Btn 6D is Backward
			IntakeControl = (vexRT[Btn6U] << 1) + vexRT[Btn6D];

			switch( IntakeControl )
			{
			case 	1: 	// Btn 6U Pressed
				RunFeed(127);		//feed forward
				break;
			case 2: 	// Btn 6D pressed
				RunFeed(-127);	//feed backwards
				break;
			default: //else
				RunFeed(0);			//feed stop
				break;
			}

			wait1Msec(15);
		}
}
