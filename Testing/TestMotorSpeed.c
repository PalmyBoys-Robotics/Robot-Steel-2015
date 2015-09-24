#pragma config(Sensor, dgtl1,  rightEncoder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  leftEncoder,    sensorQuadEncoder)
#pragma config(Motor,  port1,           rightd,        tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           leftd,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           leftf1,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           leftf2,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           rightf1,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           rightf2,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           feed,          tmotorVex393_MC29, openLoop)

int waitTime = 250;

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

int MaxSpeedR = 50;
int MaxSpeedL = 50;

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
	// InitializeFlywheelVariables();

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

task main()
{
	startTask(flywheelRamp);
	while(true)
	{
		float startCountR = SensorValue(rightEncoder);
		float startCountL = SensorValue(leftEncoder);
		wait1Msec(waitTime);
		
		float EndCountR = SensorValue(rightEncoder);
		float EndCountL = SensorValue(leftEncoder);

		float speedL = ((EndCountR - startCountR) / waitTime);
		float speedR = ((EndCountL - startCountL) / waitTime);
		writeDebugStreamLine("speed L %d", speedL);
		writeDebugStreamLine("speed R %d", speedR);
		int time = time1[T1];
		writeDebugStreamLine("Time: %d", time);

	}

}
