
// Runs Feed at specified Speed
void RunFeed(int speed)
{
	motor[feed] = -speed;
}

void RunIntake(int speed)
{
	motor[intake] = speed;
}

// Pointers to the struct in flywheelPID
float *errorR =  &fwR.error;
float *errorL =  &fwL.error;

void updateErrorPointer()
{
	errorR =  &fwR.error;
	errorL =  &fwL.error;
}

// @about Intake Control
task IntakeSystem()
{
	while(true)
	{
		// Feed Button Control
		if(vexRT[Btn6U] == 1)	{
			RunFeed(127);
		}
		else {
			RunFeed(0);
			RunIntake(0);
		}

		// Intake Button Control
		if(vexRT[Btn6D] == 1)
		{
			RunIntake(127);
		}
		else
		{
			RunFeed(0);
			RunIntake(0);
		}

		updateErrorPointer();

		// Checks if the the robot is in autonomous
		if(Comp_Control == Autonomous)
		{
			// Checks PID error is null
			if(*errorR == 0 && *errorL == 0)
			{
				// Runs the intake devices untill the speed is updated
				RunFeed(127);
				RunIntake(127);
			}
		}
		// Dont Hog CPU
		wait1Msec(20);
	}
}
