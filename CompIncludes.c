//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                       VEX Competition Control Include File
//
// This file provides control over a VEX Competition Match. It should be included in the user's
// program with the following line located near the start of the user's program
//        #include "VEX_Competition_Includes.h"
// The above statement will cause this program to be included in the user's program. There's no
// need to modify this program.
//////////////////////////////////////////////////////////////////////////////////////////////////////

#include "FirmwareVersion.h"

// local task for displaying status on the LCD display
task DisplayStatus();

// timer variable so we can see how long we have been in the current state
static int compTimer = 0;

void allMotorsOff();
void allTasksStop();
void pre_auton();
task autonomous();
task usercontrol();

bool bStopTasksBetweenModes = true;

task main()
{
	// Master CPU will not let competition start until powered on for at least 2-seconds
	wait1Msec(2000);

	pre_auton();

	while (true)
	{

		if (bIfiAutonomousMode)
			{

				// Waiting for autonomous phase to end
				while (bIfiAutonomousMode && !bIfiRobotDisabled)
					{
						if (!bVEXNETActive)
						{
						  if (nVexRCReceiveState == vrNoXmiters) // the transmitters are powered off!!
							  allMotorsOff();
						}
						wait1Msec(25);               // Waiting for autonomous phase to end
					}
			  allMotorsOff();
			  if(bStopTasksBetweenModes)
				  {
					  allTasksStop();
					}
			}

		else
		{

			// Here we repeat loop waiting for user control to end and (optionally) start
			// of a new competition run
			while (!bIfiAutonomousMode && !bIfiRobotDisabled)
			{
				if (nVexRCReceiveState == vrNoXmiters) // the transmitters are powered off!!
					allMotorsOff();
				wait1Msec(25);
		  }
			allMotorsOff();
		  if(bStopTasksBetweenModes)
		  {
			  allTasksStop();
			}
		}
	}
}

task DisplayStatus()
{
	while(true)
    {

		clearLCDLine(0);
		clearLCDLine(1);

	    // Display timer on upper line
		displayLCDPos(0, 11);
	    displayNextLCDNumber(compTimer / 600, 2);
		displayNextLCDChar(':');
		displayNextLCDNumber((compTimer / 10) % 60, -2);
	}
}


void allMotorsOff()
{
	motor[port1] = 0;
	motor[port2] = 0;
	motor[port3] = 0;
	motor[port4] = 0;
	motor[port5] = 0;
	motor[port6] = 0;
	motor[port7] = 0;
	motor[port8] = 0;
#if defined(VEX2)
	motor[port9] = 0;
	motor[port10] = 0;
#endif
}

void allTasksStop()
{
  stopTask(1);
  stopTask(2);
  stopTask(3);
  stopTask(4);
#if defined(VEX2)
  stopTask(5);
  stopTask(6);
  stopTask(7);
  stopTask(8);
  stopTask(9);
  stopTask(10);
  stopTask(11);
  stopTask(12);
  stopTask(13);
  stopTask(14);
  stopTask(15);
  stopTask(16);
  stopTask(17);
  stopTask(18);
  stopTask(19);
#endif
}
