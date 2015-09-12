#pragma config(Motor,  port1,           rightd,        tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           leftd,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           leftf1,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           leftf2,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           rightf1,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           rightf2,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           feed,          tmotorServoContinuousRotation, openLoop)

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)

// #include "SmartMotorLib.c" 					//Smart Motor Library By James Pearman
#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

//motorOFF const
const int MotorOFF = 0;

//flywheel automatic speed control
bool manualFlyWheelSpeedControl = false;

//Flywheel Definitions

int speedVariableR;
int speedVariableL;

int MaxSpeedR;
int MaxSpeedL;

int StartSpeedR;
int StartSpeedL;

// 1 will take 30 * 127 (maxspeed / IncremntRate)
// = 3800ish milisecs
int IncremntRateR;
int IncremntRateL;

bool flywheelRampActiveR;
bool flywheelRampActiveL;

int kNumbOfAttempts;

task InitializeFlywheel
{
	//reset to automatic mode
	manualFlyWheelSpeedControl = false;
	
	MaxSpeedR = 127;
	MaxSpeedL = 127;

	StartSpeedR = 0;
	StartSpeedL = 0;

	// 1 will take 30 * 127 (maxspeed / IncremntRate)
	// = 3800ish milisecs
	IncremntRateR = 1;
	IncremntRateL = 1;
	
	kNumbOfAttempts = 0;
	
	EndTimeSlice();
}

//Flywheel Definitions End
task flywheelRamp
{

	startTask(InitializeFlywheel);

	while(true)
	{
		/* This code is over complicated and could be massivally simplified. */
		/* But I wanted to make sure your could ajust every aspect of the    */
		/* flywheel for fine tuning.																				 */

		//Right Side Flywheel
		motor[rightf1] = speedVariableR;
		motor[rightf2] = speedVariableR;
		wait1Msec(50);

		if(speedVariableR < MaxSpeedR && manualFlyWheelSpeedControl == false)
		{
			//The 'Long' method shown here allows greater flexabillity wiht increment rates
			speedVariableR = speedVariableR + IncremntRateR;		//set IncremntRateR to higher for faster ramp
		}

		//This code is far from perfect, but is designed so if you
		//change the default speed starting varb the bool will still be activated :)
		if(speedVariableR == 50) { flywheelRampActiveR = true; }
		
		//Left Side FLywheel
		motor[leftf2] = speedVariableL;
		motor[leftf1] = speedVariableL;

		if(speedVariableL < MaxSpeedL && manualFlyWheelSpeedControl == false)
		{
			//The 'Long' method shown here allows greater flexabillity wiht increment rates
			speedVariableL = speedVariableL + IncremntRateL;		//set IncrementRateL to higher for faster ramp
		}
		
		if(speedVariableL == 50) { flywheelRampActiveL = true; }
		wait1Msec(25);
	}
}

task flyWheelTerminate
{
	motor[rightf2] = MotorOFF;
	motor[rightf1] = MotorOFF;
	motor[leftf2] = MotorOFF;
	motor[leftf1] = MotorOFF;
	EndTimeSlice();
}

task TankDriveMovement
{
	while(true)
	{
		motor[leftd] = vexRT[Ch3];
		motor[rightd] = vexRT[Ch2];
		wait1Msec(20);
	}
}

void RunFeed(int speed)
{
	motor[feed] = -speed;
}


void pre_auton()
{

	// Set bStopTasksBetweenModes to false if you want to keep user created tasks running between
	// Autonomous and Tele-Op modes. You will need to manage all user created tasks if set to false.
	bStopTasksBetweenModes = true;
}
///////////////////////////////////////////////////////////////////
task autonomous()
{
	startTask(flywheelRamp);

	if(speedVariableL == MaxSpeedL && speedVariableR == MaxSpeedR)
	{
		RunFeed(127);
	}

	//dont hog CPU
	wait1Msec(30);
}

//////////////////////////////////////////////////////////////////////////////////
task usercontrol()
{

	startTask(TankDriveMovement, 70);
	while (true)
	{

		//Start the ramp up flywheel
		if(vexRT[Btn5U] == 1) { startTask(flywheelRamp, 80); }

		//Termiante Switch, runs task which terminates flywheel and stops flywheelramp
		if (vexRT[Btn5D] == 1)
		{
			startTask(flyWheelTerminate);
			stopTask(flywheelRamp);
		}
		
		//Toggle FlywheelSpeed (will be reloaded by initalize)
		if(vexRT[Btn8U] == 1 && (flywheelRampActiveL == true || flywheelRampActiveR == true) && kNumbOfAttempts == 0)
			{	kNumbOfAttempts ++; speedVariableL = 51; speedVariableR = 51;	manualFlyWheelSpeedControl = true;}
		else if(vexRT[Btn8U] == 1 && (flywheelRampActiveL == true || flywheelRampActiveR == true) && kNumbOfAttempts == 1)
			{	kNumbOfAttempts ++; speedVariableL = 85; speedVariableR = 85;	manualFlyWheelSpeedControl = true;}
		else if(vexRT[Btn8U] == 1 && (flywheelRampActiveL == true || flywheelRampActiveR == true) && kNumbOfAttempts == 2)
			{	kNumbOfAttempts ++; speedVariableL = 100; speedVariableR = 100;	manualFlyWheelSpeedControl = true;}
		else if(vexRT[Btn8U] == 1 && (flywheelRampActiveL == true || flywheelRampActiveR == true) && kNumbOfAttempts == 3)
			{	kNumbOfAttempts = 0; speedVariableL = 127; speedVariableR = 127;	manualFlyWheelSpeedControl = true;}
		
		//Toggle FlywheelSpeed Down (will be reloaded by initalize)
		if(vexRT[Btn8D] == 1 && (flywheelRampActiveL == true || flywheelRampActiveR == true) && kNumbOfAttempts == 0)
			{	kNumbOfAttempts = 3; speedVariableL = 51; speedVariableR = 51; manualFlyWheelSpeedControl = true;}
		else if(vexRT[Btn8D] == 1 && (flywheelRampActiveL == true || flywheelRampActiveR == true) && kNumbOfAttempts == 1)
			{ kNumbOfAttempts --; speedVariableL = 85; speedVariableR = 85; manualFlyWheelSpeedControl = true;}
		else if(vexRT[Btn8D] == 1 && (flywheelRampActiveL == true || flywheelRampActiveR == true) && kNumbOfAttempts == 2)
			{	kNumbOfAttempts --; speedVariableL = 100; speedVariableR = 100; manualFlyWheelSpeedControl = true;}
		else if(vexRT[Btn8D] == 1 && (flywheelRampActiveL == true || flywheelRampActiveR == true) && kNumbOfAttempts == 3)
			{ kNumbOfAttempts --; speedVariableL = 127; speedVariableR = 127; manualFlyWheelSpeedControl = true;}
		
		//unlimited sideways flywheel ajustment Right
		if(vexRT[Btn8R] == 1 && flywheelRampActiveL == true && flywheelRampActiveR == true)
			{ speedVariableR = speedVariableR + 5; speedVariableL = speedVariableL - 5; manualFlyWheelSpeedControl = true;}
		
		//unlimited sideways flywheel ajustment Left
		if(vexRT[Btn8L] == 1 && flywheelRampActiveL == true && flywheelRampActiveR == true)
			{ speedVariableR = speedVariableR - 5; speedVariableL = speedVariableL + 5; manualFlyWheelSpeedControl = true;}
	
		//reset motorspeed to max
		if(vexRT[Btn7D] == 1 && (flywheelRampActiveL == true || flywheelRampActiveR == true))
			{	speedVariableL = MaxSpeedL; speedVariableR = MaxSpeedR;	kNumbOfAttempts = 0;}
		
		// feed
		//condesnsed
		if(vexRT[Btn6U] == 1) { motor[feed] = 127; }
		else if(vexRT[Btn6D] == 1) { motor[feed] = -127; }
		else { motor[feed] = 0; }
		
		wait1Msec(15);
	}
}
