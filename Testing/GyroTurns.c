#pragma config(Sensor, in1,    ,               sensorGyro)

// @about initalizes gyro
// @from NifftyFunctionLib
void InitalizeGyro(int portIndex)
	{
		SensorType[ portIndex ] = sensorNone;
		wait1Msec(2000);
		SensorType[ portIndex ] = sensorGyro;

		writeDebugStream("Gyro Initalized");
	}

// @about Checks if Gyro has been Initalized
bool GyroInitalized()
{
	return(GyroInitalizeFinished);
	GyroInitalizeFinished = false;
}

task main()
 {
	
  InitalizeGyro( in1 );
  if(GyroInitalized() == true)
  {
  	
	 //Adjust SensorScale to correct the scaling for gyro
	 //SensorScale[in8] = 260;
	 //Adjust SensorFullCount to set the "rollover" point. 3600 sets the rollover point to +/-3600
	 //SensorFullCount[in8] = 3600;
	 
	//Specify the number of degrees for the robot to turn (1 degree = 10, or 900 = 90 degrees)
	 int degrees10 = 900;
	 
	//While the absolute value of the gyro is less than the desired rotation
	 while(abs(SensorValue[in8]) < degrees10)
	 {
	 //...continue turning
	 motor[rightd] = 50;
	 motor[leftd] = -50;
	 }
	 
	//Brief brake to stop some drift
	 motor[rightd] = -5;
	 motor[leftd] = 5;
	 wait1Msec(250);
	}
 }
