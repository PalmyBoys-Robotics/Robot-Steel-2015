
#pragma config(Sensor, dgtl1,  rightEncoder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  leftEncoder,    sensorQuadEncoder)
#pragma config(Motor,  port1,           rightd,        tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           leftd,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           leftf1,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           leftf2,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           rightf1,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           rightf2,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           intake,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          feed,          tmotorVex393_HBridge, openLoop)

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

// Enum that holds the Competiton State
enum Comp_State {
  PreAutonomous,
  Autonomous,
  DriverControl
};

// Comp Control Variable that holds competition State
Comp_State Comp_Control;

#include "Vex_Competition_Includes.c"
#include "flywheelPID.c"
#include "intake.c"
#include "drive.c"

// Flywheel Speed Controls
#define FullCourtShot 2000
#define HalfCourtShot 1000
#define CloseCourtShot 750
#define FLywheelStopped 0


void pre_auton()
{
  Comp_Control = PreAutonomous;
  bStopTasksBetweenModes = true;
}

task autonomous()
{
  Comp_Control = Autonomous;
  // Set Target to Full Court Shots
  targetVelocityRPM = FullCourtShot;

  // Starts Flywheel
  startTask( fw_PIDController, kHighPriority );
  // Runs intake if flywheel error is 0
  startTask( IntakeSystem );
}

task usercontrol()
{
  Comp_Control = DriverControl;

  // Starts Flywheel
  startTask( fw_PIDController, kHighPriority );
  // Runs intake on button press
  startTask( IntakeSystem );
  // Runs a Task to Control TankDrive
  startTask( TankDriveMovement );

	while (true)
	{

    // Startup Velocity ( Full Court )
    if(vexRT[Btn5U] == 1)
    {
      targetVelocityRPM = FullCourtShot;
    }
    // 7L is half court
    if(vexRT[Btn7L] == 1)
    {
      targetVelocityRPM = HalfCourtShot;
    }
    // 7U is Full court
    if(vexRT[Btn7U] == 1)
    {
      targetVelocityRPM = FullCourtShot;
    }
    // 7R is close court
    if(vexRT[Btn7R] == 1)
    {
      targetVelocityRPM = CloseCourtShot;
    }
    // 7D or 5D is turn off
    if(vexRT[Btn7D] == 1 || vexRT[Btn5D] == 1)
    {
      targetVelocityRPM = FlywheelStopped;
    }

    // Dont Hog CPU
    wait1Msec(20);

	}
}
