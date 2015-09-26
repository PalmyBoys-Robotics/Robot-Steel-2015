/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                        Copyright (c) John Cole                              */
/*                                   2015                                      */
/*                            All Rights Reserved                              */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*    Module:     autonomous_functions.c                                       */
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
/*    A library consisting of all of the autonomous functions and task         */
/*    defintions required for the opperation of the 2903 PNBHS steel Robot     */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

// @about Code to turn robot
// @from my own code on a patch for the Vex Robotics Lakeside 2015 file
// @url: https://github.com/EvolvedAwesome/Vex_Code/commit/35411244fc59402c16f9c5a3da01d869edb6a740
void moveTurn(int power, int time, string direction)
{

		int directionNUMB;
		if(direction == "Left") { directionNUMB = -1; }		// -1 for left
		if(direction == "Right") { directionNUMB = 1; }		// 1 for right

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
	writeDebugStreamLine("%a", debugstreamMotor);
}

// @about stop motor from moving
// @from nifftyFunctionLib
void StopMotor(int index)
{
	motor[index] = vexMotorOff;
}