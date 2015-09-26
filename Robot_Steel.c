#pragma config(Sensor, dgtl1,  rightEncoder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  leftEncoder,    sensorQuadEncoder)
#pragma config(Motor,  port1,           rightd,        tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           leftd,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           leftf1,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           leftf2,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           rightf1,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           rightf2,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           loader,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           intake,          tmotorVex393_MC29, openLoop)

#pragma platform(VEX)
#pragma competitionControl(Competition)

#include "drive.c"
#include "intake.c"						// Intake Library
#include "flywheel.c"					// Flywheel Library
#include "drive_selection.c"			// Drive Selection Code & Functions
#include "autonomous_functions.c"		// Autonomous movement functions
#include "CompIncludes.c"   			// Main competition background code

#define vexMotorOff 0
#define vexMotorMax 127
#define vexMotorMin -127

#define MOTOR_FLYRIGHT1 rightf1
#define MOTOR_FLYRIGHT2 rightf2
#define MOTOR_FLYLEFT1 leftf1
#define MOTOR_FLYLEFT2 leftf2

// Buttons to start & stop the flywheel withn the FlyTerminateSwitchTask
#define TERMINATE_FLYWHEEL_BUTTON Btn5D
#define START_FLYWHEEL_BUTTON Btn5U

// Buttons to control the Loader and Intake going forward
#define LOADER_BUTTON Btn6U
#define INTAKE_BUTTON Btn6D

// The Gear Ratio of your flywheel e.g. 27:1 would be 27 (for scaling)
#define FlyGearRatio 27

// Debug mode writes debug info to debug stream
#define DEBUG_MODE false

// The highest speed you want the flywheel to go
int MaxSpeedR = 70;
int MaxSpeedL = 70;

// The highest speed you want the flywheel to go
float EncoderTargetVelocityR = 2000;				// in RPMs
float EncoderTargetVelocityL = 2000;

// The Start Speed of the flywheel
const int StartSpeedR 0
const int StartSpeedL 0

// The ammout that the motor speed is increased each loop
const int IncremntRateR = 1;
const int IncremntRateL = 1;



void pre_auton()
{

	SelectDriveTypeLCD();

	bStopTasksBetweenModes = true;
}


task autonomous()
{
	startTask(DisplayStatus, kLowPriority );
	startTask(flywheelRamp, kHighPriority );

	startTask(AutoRunFeeder);
}


task usercontrol()
{
	CheckDriveType();

	startTask(flywheelRamp, kHighPriority );
	startTask(FlyTerminateSwitch);
	startTask(IntakeControls);
	startTask(DisplayStatus, kLowPriority );
		 
}
