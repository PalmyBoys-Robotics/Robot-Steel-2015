/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                        Copyright (c) John Cole                              */
/*                                   2015                                      */
/*                            All Rights Reserved                              */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*    Module:     drive.c                                                      */
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
/*    A library consisting of all of the drive functions and task              */
/*    defintions required for the opperation of the 2903 PNBHS steel Robot     */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

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