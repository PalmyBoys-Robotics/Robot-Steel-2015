
float Channel2;
float Channel3;
float threshold = 15;

// Creates a deadzone for joystick
void deadZoneCheck()
{
	// Create deadzone for Ch2
	if(abs(vexRT[Ch2]) > threshold)
		Channel2 = vexRT[Ch2];
	else
		Channel2 = 0;

	// Create deadzone for Ch3
	if(abs(vexRT[Ch3]) > threshold)
		Channel3 = vexRT[Ch3];
	else
		Channel3 = 0;
}

// Driver control TankDrive task
task TankDriveMovement
{
	while(true)
	{
		deadZoneCheck();
		motor[leftd] = vexRT[Channel3];
		motor[rightd] = vexRT[Channel2];

		// Dont Hog CPU
		wait1Msec(20);
	}
}
