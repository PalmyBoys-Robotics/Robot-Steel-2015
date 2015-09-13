

#define vexMotorOFF 0
#define vexMotorMAX 127
#define vexMotorMIN -127

int MaxSpeedRMASTER;		//Change these, change the world (or at least the flywheel code)
int MaxSpeedLMASTER;		
int StartSpeedRMASTER;
int StartSpeedLMASTER;
int IncremntRateRMASTER;
int IncremntRateLMASTER;

bool flywheelRampActiveR;
bool flywheelRampActiveL;
bool DebugMode = true;

//////////////////////////////////////////////////////////////////////////////////////////////
	// Base Functions

void Equalise(int Numb1, int Numb2)
	{
		Numb1 = Numb2;				//Make Numb 1 the value of Numb 2
	}

float average(int Num1, int Num2)
	{
		float value;
		value = Num1 + Num2 / 2;
		return(value);
	}
	
//Get Current Motor Speed
int getCurrentMotorSpeed(int portIndex)
	{
		int CurrentMotorSpeed;
		CurrentMotorSpeed = motor[portIndex];
		return(CurrentMotorSpeed);
	}

//Check For Click
bool CheckforClick(int BtnIndex)
	{
			bool click = false;
			int CurrentBtn;
			int PrevBtn;
	
	     PrevBtn = CurrentBtn ;      //will set the previous state to the "current" which hasnt been updated yet
	     CurrentBtn = vexRT[BtnIndex];       //updates the current state of button 5
	
	     if(CurrentBtn == 1 && PrevBtn == 0 )  // if btn 5 is pressed now and wasnt pressed last frame
	     {
	           click = true; 					//was clicked
	           writeDebugStreamLine("Clicked: ", BtnIndex);
	     }
	
	     // will have to use if (CheckforClick(Btn8U) == true)
	     return( click );								//was not clicked
	}

//Autonomous Move motor with time control
void moveMotor(int index, int speed, int time1Msec)
	{
		motor[index] = speed;
		wait1Msec(time1Msec);
		
		string debugstreamMotor;
		sprintf(debugstreamMotor,"Motor: ", index, "Went:", speed, "speed for: ", time1Msec, "Msecs",); //Build the value to be displayed
		writeDebugStream(debugstreamMotor);
	}

bool ArcadeDriveCurrent;
bool TankDriveCurrent;

//Driver control
void TankDrive(int index1, int index2, int waitTime = 0)
	{
		motor[index1] = vexRT[Ch3];
		motor[index2] = vexRT[Ch2];
		wait1Msec(waitTime);
		if(TankDriveCurrent == false) 	{ TankDriveCurrent = true; }
		if(ArcadeDriveCurrent == true) 	{ ArcadeDriveCurrent = false; }
	}

//Arcade Control
void ArcadeDrive(int index1, int index2, int waitTime = 0)
	{
		motor[index1] = (vexRT[Ch2] + vexRT[Ch1])/2;
		motor[index2] = (vexRT[Ch2] - vexRT[Ch1])/2;
		wait1Msec(waitTime);
		if(TankDriveCurrent == true) 		{ TankDriveCurrent = false; }
		if(ArcadeDriveCurrent == false) { ArcadeDriveCurrent = true; }
	}

//zero a int (elegant soloution)
void zeroInt(int intIndex)
	{
		intIndex = 0;
	}

//does the same thing, used with motors
//turn off motor
void StopMotor(int index)
	{
		motor[index] = vexMotorOFF;
	}

	
//returns true if a value is below -127
bool BelowVexMotorMin(int intIndex)
	{
		bool BelowVexMotorMin = false;
		if(intIndex < vexMotorMIN)
			{
				BelowVexMotorMin = true;
				if(DebugMode == true) {writeDebugStream("Int below vexMotorMIN");}
			}
		return (BelowVexMotorMin);
	}

//returns true if a value is above 127
bool AboveVexMotorMin(int intIndex)
	{
		bool AboveVexMotorMin = false;
		if(intIndex > vexMotorMAX)
			{
				AboveVexMotorMin = true;
				if(DebugMode == true) {writeDebugStream("Int above vexMotorMAX");}
			}
		return (AboveVexMotorMin);
	}

void InitalizeLCD()
	{
		clearLCDLine(0);
  	clearLCDLine(1);

  	bLCDBacklight = true;

  	if(DebugMode == true) {writeDebugStream("LCD Initalized");}
  }

void InitalizeGyro(int portIndex)
	{
		SensorType[ portIndex ] = sensorNone;
		wait1Msec(1000);
		SensorType[ portIndex ] = sensorGyro;

		if(DebugMode == true) {writeDebugStream("Gyro Initalized");}
	}

void ClearIME(int portIndex)
	{
		nMotorEncoder[portIndex] = 0;

		if(DebugMode == true) {writeDebugStream("IME Cleared");}
	}

void ClearShaftEncoder(int portIndex)
	{
		SensorValue[portIndex] = 0;

		if(DebugMode == true) {writeDebugStream("Shaft Encode Cleared");}
	}

///////////////////////////////////////////////////////////////////////////////////////
	// Advanced Functions

bool feedactive = false;
//Run the feed
void Run_Feed(int feedindex,int speed)
	{
		motor[feedindex] = (speed);
		if(speed > 0 || speed < 0) { feedactive = true; writeDebugStream("feed currently active"); }
		if(speed == 0) {feedactive = false; writeDebugStream("feed not active, to move feed set speed value above 0"); }
	}

//Bool whether feed is active
bool feedCurrentlyActive()
	{
		if(feedactive == true)	{ return true; }
		else 										{ return false; }
	}

//Display Current Drive Type in string for LCD usage
char * CurrentDriveType()
	{
			if(TankDriveCurrent == true) 					{ return "TankDrive"; }
			else if(ArcadeDriveCurrent == true) 	{ return "ArcadeDrive"; }
			else { return "None"; if(DebugMode == true) {writeDebugStream("No Motor Installed");} }
	}
		
/////////////////////////////////////////////////////////////////////////////////////////
	//Tasks with definitions

//Install Motors for Tasks
int DriveMotorRight;
int DriveMotorRight2;
int DriveMotorLeft;
int DriveMotorLeft2;

bool TwoMotorDriveInstalled = false;
bool FourMotorDriveInstalled = false;

void install2MotorDrive(int motorIndexR = port2, int motorIndexL = port3)
	{
		motorIndexR = DriveMotorRight;
		motorIndexL = DriveMotorLeft;
		TwoMotorDriveInstalled = true;
		if(DebugMode == true) {writeDebugStream("2 Motor Drive Succesfully Installed");}
	}

void install4MotorDrive(int motorIndexR, int motorIndexR2, int motorIndexL, int motorIndexL2)
	{
		motorIndexR = DriveMotorRight;
		motorIndexR = DriveMotorRight2;
		motorIndexL = DriveMotorLeft;
		motorIndexL2 = DriveMotorLeft2;
		FourMotorDriveInstalled = true;
		if(DebugMode == true) {writeDebugStream("4 Motor Drive Succesfully Installed");}
	}

//Driver control task
task TankDriveMovement
	{
		if(TwoMotorDriveInstalled == true)
			{
				while(true)
					{
						motor[DriveMotorRight] = vexRT[Ch3];		//Channel 3 is Right
						motor[DriveMotorLeft] = vexRT[Ch2];			//Channel 2 is Left
						wait1Msec(20);
					}
			}
		else if(FourMotorDriveInstalled == true)
			{
				while(true)
					{
						int RightDrive = vexRT[Ch3];
						motor[DriveMotorRight] = RightDrive;
						motor[DriveMotorRight2] = RightDrive;

						int LeftDrive = vexRT[Ch2];
						motor[DriveMotorLeft] = LeftDrive;
						motor[DriveMotorRight] = LeftDrive;
					}
			}
	}

task ArcadeDriveMovement
	{
		if(TwoMotorDriveInstalled == true)
			{
				while(true)
					{
						motor[DriveMotorRight] = (vexRT[Ch2] + vexRT[Ch1])/2;
						motor[DriveMotorLeft] = (vexRT[Ch2] - vexRT[Ch1])/2;
						wait1Msec(20);
					}
			}
		else if(FourMotorDriveInstalled == true)
			{
				while(true)
					{
						int RightDrive = (vexRT[Ch2] + vexRT[Ch1])/2;
						motor[DriveMotorRight] = RightDrive;
						motor[DriveMotorRight2] = RightDrive;

						int LeftDrive = (vexRT[Ch2] - vexRT[Ch1])/2;
						motor[DriveMotorLeft] = LeftDrive;
						motor[DriveMotorRight] = LeftDrive;
					}
			}
		else
			{
				writeDebugStream("No Drive Motor Installed, Please Install Motor Using install2MotorDrive or install4MotorDrive");
			}
	}
	
	//found this function useful for debugging	
char * getCurrentDriveMotors()
	{
		if(TwoMotorDriveInstalled == true)
			{
				string TwoMotorDriveValues;
				sprintf(TwoMotorDriveValues, ":", DriveMotorRight, DriveMotorLeft);
				writeDebugStreamLine(TwoMotorDriveValues);
				return(TwoMotorDriveValues);
			}
		else if(FourMotorDriveInstalled == true)
			{
				string FourMotorDriveValues;
				sprintf(FourMotorDriveValues, ":", DriveMotorRight, DriveMotorRight2, DriveMotorLeft, DriveMotorLeft2);
				writeDebugStreamLine(FourMotorDriveValues);
				return(FourMotorDriveValues);
			}
		else
			{
				writeDebugStream("No Drive Motor Installed");
				return("No Drive Motor Installed");
			}
		}
		
///////////////////////////////////////////////////////////////////////////////
		//Flywheel Code
int MaxSpeedR;
int MaxSpeedL;

int IncremntRateL;
int IncremntRateR;

int StartSpeedL;
int StartSpeedR;
	
//Setup Variables and Master Variables		
void FlyWheelRampFunctionsMaxSpeed(int MaxSpeedR1, int MaxSpeedL1)
	{
		Equalise(MaxSpeedR1, MaxSpeedRMASTER);
		Equalise(MaxSpeedL1, MaxSpeedLMASTER);
	}
	
void FlyWheelRampFunctionsStartSpeed(int StartSpeedR1, int StartSpeedL1)
	{
		Equalise(StartSpeedRMASTER, StartSpeedR1);
		Equalise(StartSpeedLMASTER, StartSpeedL1);
	}
	
void FlyWheelRampFunctionsIncrementRate(int IncremntRateR1, int IncremntRateL1)
	{
		Equalise(IncremntRateR1, IncremntRateRMASTER);
		Equalise(IncremntRateL1, IncremntRateLMASTER);
	}
		
// flywheel automatic speed control
bool manualFlyWheelSpeedControl = false;

// int used in ToggleFlywheelSpeed up & down as simple way of creating a toggle button
int kNumbOfAttempts;

//Equalise Make Variables equal to master counterparts 
void InitializeFlywheel()
	{
		// reset to automatic mode
		manualFlyWheelSpeedControl = false;
	
		int MaxSpeedR;
		Equalise(MaxSpeedR, MaxSpeedRMASTER);
		int MaxSpeedL;
		Equalise(MaxSpeedL, MaxSpeedLMASTER);
		
		int StartSpeedR;
		Equalise(StartSpeedR, StartSpeedRMASTER);
		int StartSpeedL;
		Equalise(StartSpeedL, StartSpeedLMASTER);
		
		int IncremntRateR;
		Equalise(IncremntRateR, IncremntRateRMASTER);
		int IncremntRateL;
		Equalise(IncremntRateL, IncremntRateLMASTER);
		
		//Flywheel Toggle Reset
		kNumbOfAttempts = 0;
	}

//global motor index values
char MotorFlyR1PORT;
char MotorFlyR2PORT;
char MotorFlyL1PORT;
char MotorFlyL2PORT;

//Setup FlyWheelMotors
void install4MotorFlyWheel(char MotorIndexR1, char MotorIndexR2, char MotorIndexL1, char MotorIndexL2)
	{
		MotorIndexR1 = MotorFlyR1PORT;
		MotorIndexR2 = MotorFlyR2PORT;
		
		MotorIndexL1 = MotorFlyL1PORT;
		MotorIndexL1 = MotorFlyL2PORT;
	}

	
int speedVariableRTMP;
int speedVariableLTMP;
	
task flywheelRamp
{

	//makes sure all flywheel values are default before starting
	InitializeFlywheel();

	while(true)
	{
		/// This code is over complicated and could be massivally simplified. ///
		/// But I wanted to make sure your could ajust every aspect of the    ///
		/// flywheel for fine tuning.					      ///

		// Both Sides of the flywheel are individualy controlled, because of this
		// they can be individually finetuned, monitored and changed so you can
		// get 'the perfect shot' every time. Its likely that encoders will be
		// added below to make sure the wheels are at very similar speeds
		// throughout the match

		//Right Side Flywheel
		motor[MotorFlyR1PORT] = speedVariableRTMP;
		motor[MotorFlyR1Port] = speedVariableRTMP;
		wait1Msec(50);

		if(speedVariableRTMP < MaxSpeedR && manualFlyWheelSpeedControl == false)
		{
			//The 'Long' method shown here allows greater flexabillity wiht increment rates
			speedVariableRTMP = speedVariableRTMP + IncremntRateR;		//set IncremntRateR to higher for faster ramp
		}

		//This code is far from perfect, but is designed so if you
		//change the default speed starting varb the bool will still be activated :)
		if(speedVariableRTMP == 50) { flywheelRampActiveR = true; }

		//Left Side FLywheel
		motor[MotorFlyL1PORT] = speedVariableLTMP;
		motor[MotorFlyL2PORT] = speedVariableLTMP;

		if(speedVariableLTMP < MaxSpeedL && manualFlyWheelSpeedControl == false)
		{
			//The 'Long' method shown here allows greater flexabillity wiht increment rates
			speedVariableLTMP = speedVariableLTMP + IncremntRateL;		//set IncrementRateL to higher for faster ramp
		}

		if(speedVariableLTMP >= 25) { flywheelRampActiveL = true; }
		wait1Msec(25);

	}
}
