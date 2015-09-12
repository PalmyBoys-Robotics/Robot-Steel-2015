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
//defined in initalizefLyWheel Task

int speedVariableR;
int speedVariableL;

int MaxSpeedR;
int MaxSpeedL;

int StartSpeedR;
int StartSpeedL;

int IncremntRateR;
int IncremntRateL;

bool flywheelRampActiveR;
bool flywheelRampActiveL;

//int used in ToggleFlywheelSpeed up & down as simple way of creating a toggle button
int kNumbOfAttempts;

//run to initalize flywheel and reset values to default
//*default values set here*
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
	
	//dont hog cpu
	EndTimeSlice();
}

//Flywheel Definitions End
task flywheelRamp
{
	
	//makes sure all flywheel values are default before starting
	startTask(InitializeFlywheel);

	while(true)
	{
		/// This code is over complicated and could be massivally simplified. ///
		/// But I wanted to make sure your could ajust every aspect of the    ///
		/// flywheel for fine tuning.					      ///
		
		// Both Sides of the flywheel are individualy controlled, because of this
		// they can be individually finetuned, monitored and changed so you can
		// get 'the perfect shot' every time. Its likely that encoders will be
		// added below to make sure the wheels are at very similar speeds
		// throughout the match

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

// Terminates Flywheel immeadeatly 
task flyWheelTerminate
{
	motor[rightf2] = MotorOFF;
	motor[rightf1] = MotorOFF;
	motor[leftf2] = MotorOFF;
	motor[leftf1] = MotorOFF;
	EndTimeSlice();
}

//Driver control task
task TankDriveMovement
{
	while(true)
	{
		motor[leftd] = vexRT[Ch3];
		motor[rightd] = vexRT[Ch2];
		wait1Msec(20);
	}
}

//Runs Feed
void RunFeed(int speed)
{
	motor[feed] = -speed;
}


void pre_auton()
{
	//encoder initalization will occur here
	bStopTasksBetweenModes = true;
}
///////////////////////////////////////////////////////////////////
task autonomous()
{
	startTask(flywheelRamp);
	
	while(true)
	{
		//although this if statement only checks software motor speeds
		if(speedVariableL == MaxSpeedL && speedVariableR == MaxSpeedR)
		{
			RunFeed(127);
		}
		//dont hog CPU
		wait1Msec(30);
	}
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
		
		// The Two 'paragraphs' of code below are based of the idea of a toggle, the reason I did this is 
		// I wanted to be able to move between several different states instead of simply incrasing or decreasing
		// the speed of the flywheels. When encoders are intergrated this code will have to be ajusted.
		// Could also add current ajustment!
		
		//Toggle FlywheelSpeed Up (will be reloaded by initalize)
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
		
		// Now this code will ajust the speed of the flywheels in either direction for ever!
		
		//unlimited sideways flywheel ajustment Right
		if(vexRT[Btn8R] == 1 && flywheelRampActiveL == true && flywheelRampActiveR == true)
			{ speedVariableR = speedVariableR + 5; speedVariableL = speedVariableL - 5; manualFlyWheelSpeedControl = true;}
		
		//unlimited sideways flywheel ajustment Left
		if(vexRT[Btn8L] == 1 && flywheelRampActiveL == true && flywheelRampActiveR == true)
			{ speedVariableR = speedVariableR - 5; speedVariableL = speedVariableL + 5; manualFlyWheelSpeedControl = true;}
	
		//this is a non initilazion quick motorspeed reset
		//reset motorspeed to max
		if(vexRT[Btn7D] == 1 && (flywheelRampActiveL == true || flywheelRampActiveR == true))
			{	speedVariableL = MaxSpeedL; speedVariableR = MaxSpeedR;	kNumbOfAttempts = 0;}
		
		//condesned feed
		if(vexRT[Btn6U] == 1) { motor[feed] = 127; }
		else if(vexRT[Btn6D] == 1) { motor[feed] = -127; }
		else { motor[feed] = 0; }
		
		wait1Msec(15);
	}
}
