/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                        Copyright (c) John Cole                              */
/*                                   2015                                      */
/*                            All Rights Reserved                              */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*    Module:     flywheel.c                                                   */
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
/*    A library consisting of all of the flywheel functions and task           */
/*    defintions required for the opperation of the 2903 PNBHS steel Robot     */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

// The variable that holds the flywheels current speed
float speedVariableR;
float speedVariableL;

// @about Checks if inputted int is above 127 or below -127
void checkVexMotorSpec(int intIndex)
{

	if(intIndex > vexMotorMax)
	{
		intIndex = 127;
	}

	if(intIndex < vexMotorMin)
	{
		intIndex = -127;
	}

}

// @about gets the current Right RPM of the flywheel (scaled with gear ratio)
int getFlyRPML( int waitTime = 150 )
{
	float startCountL = SensorValue(leftEncoder);

	wait1Msec(waitTime);

	float EndCountL = SensorValue(leftEncoder);

	float speedL = ((EndCountR - startCountR) / waitTime) * 60 * FlyGearRatio;
	return speedL;
}

// @about gets the current Left RPM of the flywheel (scaled with gear ratio)
int getFlyRPMR( int waitTime = 150 )
{
	float startCountR = SensorValue(rightEncoder);

	wait1Msec(waitTime);
		
	float EndCountR = SensorValue(rightEncoder);

	float speedR = ((EndCountL - startCountL) / waitTime) * 60 * FlyGearRatio;
	return speedR;
}

// @about Reset speed Variable int
void InitializeFlywheelVariables()
{
	// The Starting speed of the flywheels
	speedVariableR = StartSpeedR;
	speedVariableL = StartSpeedL;
}

// @about Sets the speed of the flywheel
void flywheel_SetSpeedR( int speed )
{
	MOTOR_FLYRIGHT1 = speed;
	MOTOR_FLYRIGHT2 = speed;
	writeDebugStreamLine("FlySpeed Right = %a", speed);
}

void flywheel_SetSpeedL( int speed )
{
	MOTOR_FLYLEFT1 = speed;
	MOTOR_FLYLEFT2 = speed;
	writeDebugStreamLine("FlySpeed Left = %a", speed);
}

// @about Flywheel ramp from startSpeed to MaxSpeed
// @from written for Steel originally, but transfered to NifftyFlywheelLib.c
task flywheelRamp
{

	if(vexRT[START_FLYWHEEL_BUTTON] == 1)
	{
		// resets speedVariable
		InitializeFlywheelVariables();
	
		while(true)
		{
	
			// incremnt motor speed
			if(speedVariableR < MaxSpeedR) { speedVariableR += IncremntRateR; }
			if(speedVariableL < MaxSpeedL) { speedVariableL += IncremntRateL; }
	
			flywheel_SetSpeedR(speedVariableR);
			flywheel_SetSpeedL(speedVariableL);

			checkVexMotorSpec(speedVariableL);
			checkVexMotorSpec(speedVariableR);

		//dont hog cpu
		wait1Msec(75);
	}
}

// @about task that termiantes the flywheel on click!
task FlyTerminateSwitch
{
	while(true)
	{
		if(vexRT[TERMINATE_FLYWHEEL_BUTTON] == true)
		{
			flywheel_SetSpeedL(0);
	    	flywheel_SetSpeedR(0);
	    }
	    
	  	wait1Msec(50);
	 }
}

task AutoRunFeeder
{
	while(true)
	{
		if(getFlyRPMR() == EncoderTargetVelocityR) && (getFlyRPML() == EncoderTargetVelocityL)
		{
			RunIntake(127);
			RunLoader(127);
		}
	}
}

