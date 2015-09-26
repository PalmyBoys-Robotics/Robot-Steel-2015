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

// The Gear Ratio to one of your flywheel (set to 1 for no scaling) e.g. 27 would be 27:1
#define FlyGearRatio 27

// The wait time for calculating flywheel speed (50-250 msecs)
int waitTime = 250;

// The Int that holds the state of the feed switch statement
int IntakeControl;

// The variable that holds the flywheels current speed
int speedVariableR;
int speedVariableL;

// The highest speed you want the flywheel to go
int MaxSpeedR = 70;
int MaxSpeedL = 70;

// The Start Speed of the flywheel
const int StartSpeedR 0
const int StartSpeedL 0

// The amout that the motor speed is increased each loop
const int IncremntRateR = 1;
const int IncremntRateL = 1;

// @about Runs Feed ast specified speed
void RunFeed(int speed)
{
	motor[feed] = -speed;
}

// @about gets the current RPM of the flywheel (scaled with gear ratio)
int getFlyRPML()
{
	float startCountL = SensorValue(leftEncoder);

	wait1Msec(waitTime);

	float EndCountL = SensorValue(leftEncoder);

	float speedL = ((EndCountR - startCountR) / waitTime) * 60 * FlyGearRatio;
	return speedL;
}

// @about gets the current RPM of the flywheel (scaled with gear ratio)
int getFlyRPMR()
{
	float startCountR = SensorValue(rightEncoder);

	wait1Msec(waitTime);
		
	float EndCountR = SensorValue(rightEncoder);

	float speedR = ((EndCountL - startCountL) / waitTime) * 60 * FlyGearRatio;
	return speedR;
}

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
	//The Starting speed of the flywheels
	speedVariableR = StartSpeedR;
	speedVariableL = StartSpeedL;

	kNumbOfAttempts = 0;
}

// @about Flywheel ramp from startSpeed to MaxSpeed
// @from written for Steel originally, but transfered to NifftyFlywheelLib.c
task flywheelRamp
{

	if(vexRT[Btn5U] == 1)
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

			//resets value if above 127 or below -127
			if(AboveVexMotorMax(speedVariableR) == true) 
			{ 
			speedVariableR = 127; 
			writeDebugStream("Motor Flywheel int = above vexMotorMax, *reset to 127"); 
			}
			
			if(AboveVexMotorMax(speedVariableL) == true) 
			{ 
			speedVariableL = 127;
			writeDebugStream("Motor Flywheel int = above vexMotorMax, *reset to 127");
			}

		//dont hog cpu
		wait1Msec(75);
	}
}

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

task feedIntakeControl
{
	while(true)
	{
	
		// @about Simple intake
		// @extraInfo uses a state machine.
		//Btn 6U is Forward			//Btn 6D is Backward
		IntakeControl = (vexRT[Btn6U] << 1) + vexRT[Btn6D];

		switch( IntakeControl )
		{
		case 	1: 			// Btn 6U Pressed
			RunFeed(127);		//feed forward
			break;
		case 2: 			// Btn 6D pressed
			RunFeed(-127);		//feed backwards
			break;
		default: //else
			RunFeed(0);		//feed stop
			break;
		}
		
		wait1Msec(15);
	}
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
		// Insert LCD Drive selection here

		bStopTasksBetweenModes = true;
	}
/////////////////////////////////////////////////////////////////////////////////////
	task autonomous()
	{
		startTask(flywheelRamp);

		//encoder feed code here
	}

/////////////////////////////////////////////////////////////////////////////////////
task usercontrol()
	{

		startTask(TankDriveMovement);
		startTask(FlyTerminateTerminateSwitch);
		startTask(flywheelRamp, 100);
		startTask(feedIntakeControl);
		
	}
