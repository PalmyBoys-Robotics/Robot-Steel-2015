
// Runs Feed at specified Speed
void RunFeed(int speed)
{
	motor[feed] = -speed;
}

void RunIntake(int speed)
{
	motor[intake] = speed;
}

// @about Intake Control
task IntakeSystem()
{
	while(true)
	{
		// Feed
		if(vexRT[Btn6U] == 1)	{	RunFeed(127);	}
		else { RunFeed(0); RunIntake(0); }

		// Intake
		if(vexRT[Btn6D] == 1) {	RunIntake(127);	}
		else { RunFeed(0); RunIntake(0); }

		// Checks PID CONTROL is a full speed and its autonomous then runs
		if((fwR.error == 0 && fwL.error = 0) && Comp_Control == Autonomous)
	  {
	    RunFeed(127);
			RunIntake(127);
	  }

		// Dont Hog CPU
		wait1Msec(20);
	}
}
