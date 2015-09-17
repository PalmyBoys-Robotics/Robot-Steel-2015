void RunFeed(int speed)
{
	motor[feed] = -speed;
}

// @about Code for running a intake fowards and backwards :) 
// @warning Not intelligent, you must set the motor (intake) to whats applicable
// @extraInfo the default spped is 127, although this can be changed
void intakeForward(int speed = 127)
{
  motor[intake] = speed;
}

void intakeBackward(int speed = 127) 
{
  motor[intake] = speed;
}
