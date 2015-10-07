/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                        Copyright (c) John Cole                              */
/*                                   2015                                      */
/*                            All Rights Reserved                              */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*    Module:     drive_selection.c                                            */
/*    Author:     John Cole                                                    */
/*    Created:    26 September 2015                                            */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*    The author is supplying this software for use with the VEX cortex        */
/*    control system. This file can be freely distributed and teams are        */
/*    authorized to freely use this program , however, it is requested that    */
/*    improvements or additions be shared with the Vex community via the vex   */
/*    forum.  Please acknowledge the work of the authors when appropriate.     */
/*    Thanks.                                                                  */
/*                                                                             */
/*    Licensed under the Apache License, Version 2.0 (the "License");          */
/*    you may not use this file except in compliance with the License.         */
/*    You may obtain a copy of the License at                                  */
/*                                                                             */
/*      http://www.apache.org/licenses/LICENSE-2.0                             */
/*                                                                             */
/*    Unless required by applicable law or agreed to in writing, software      */
/*    distributed under the License is distributed on an "AS IS" BASIS,        */
/*    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. */
/*    See the License for the specific language governing permissions and      */
/*    limitations under the License.                                           */
/*                                                                             */
/*    The author can be contacted on the vex forums as EvolvingJon             */
/*    or electronic mail using johncole.contact@gmail.com                      */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*    A library consisting of all of the drive selection functions and task    */
/*    defintions required for the opperation of the 2903 PNBHS steel Robot     */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

const short leftButton = 1;
const short centerButton = 2;
const short rightButton = 4;

// @about Wait for Press
void waitForPress()
{
	while(nLCDButtons == 0){}
	wait1Msec(5);
}


// @about Wait for Release
void waitForRelease()
{
	while(nLCDButtons != 0){}
	wait1Msec(5);
}

void SelectDriveTypeLCD()
{
	// Turn on backlight
	bLCDBacklight = true;

	// Declare count variable to keep track of our choice
	int count = 0;

	// Clear LCD
	clearLCDLine(0);
	clearLCDLine(1);

	// Loop while center button is not pressed
	while(nLCDButtons != centerButton & bIfiRobotDisabled )
	{
		// Switch case that allows the user to choose from 4 different options
		switch(count)
		{
			case 0:
				// Display Tank choice
				displayLCDCenteredString(0, "Tank Drive");
				displayLCDCenteredString(1, "<         Enter        >");
				waitForPress();

				// Change between the two screens
				if(nLCDButtons == leftButton)
				{
					waitForRelease();
					count = 1;
				}
				else if(nLCDButtons == rightButton)
				{
					waitForRelease();
					count = 1;
				}
				break;

			case 1:
				// Display Arcade choice
				displayLCDCenteredString(0, "Arcade Drive");
				displayLCDCenteredString(1, "<         Enter        >");
				waitForPress();

				// Change between the two screens
				if(nLCDButtons == leftButton)
				{
					waitForRelease();
					count = 0;
				}
				else if(nLCDButtons == rightButton)
				{
					waitForRelease();
					count = 0;
				}
				break;

			default:
				count = 0;
				break;
		}
	}
	startTask(DisplayStatus, kLowPriority );
}

void CheckDriveType();
{
	// Switch Case that runs the task associated with the drive selected
	switch(count)
	{
		case 0:
			startTask(TankDriveMovement);
			break;

		case 1:
			startTask(ArcadeDriveMovement);
			break;

		default:
			startTask(TankDriveMovement);
			break;
	}
}
