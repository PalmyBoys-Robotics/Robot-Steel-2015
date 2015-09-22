// @about task to run in background that termiantes the flywheel!

task FlyTerminateTerminate
{
  while(true)
  {
    if(vexRT[Btn5D] == true)
    {
      motor[leftf1] = 0;
      motor[leftf2] = 0;
      motor[rightf1] = 0;
      motor[rightf2] = 0;
    }
    
  wait1Msec(50);
  }
}
  
