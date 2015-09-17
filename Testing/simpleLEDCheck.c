#pragma config(Sensor, dgtl10, greenLED,       sensorLEDtoVCC)
#pragma config(Sensor, dgtl11, orangeLED,      sensorLEDtoVCC)
#pragma config(Sensor, dgtl12, redLED,         sensorLEDtoVCC)

task flashRedLED
{
	while(true)
	{
		SensorValue(redLED) = 0;
		wait1Msec(50);
		SensorValue(redLED) = 1;
		wait1Msec(50);
	}
}

//sample code
task main()
{
	//turn vexLCD on
	SensorValue(greenLED) = 0;
	SensorValue(orangeLED) = 0;
	SensorValue(redLED) = 0;
	wait1Msec(250);

	
	//turn vexLCD off
	SensorValue(greenLED) = 1;
	SensorValue(orangeLED) = 1;
	SensorValue(redLED) = 1;
	wait1Msec(250);
	
	//if 6500 is higher than battergy voltage
	if(nImmediateBatteryLevel > 6.5) { SensorValue(greenLED) = 0; }
	//if battery voltage is between 6500 and 5200
	if(nImmediateBatteryLevel < 6.5 && nImmediateBatteryLevel > 5.2 ) { SensorValue(orangeLED) = 0; }
	//if battery voltage is below 5200 flash red LED
	if(nImmediateBatteryLevel < 5.2) 
		{ startTask(flashRedLED); }

}
