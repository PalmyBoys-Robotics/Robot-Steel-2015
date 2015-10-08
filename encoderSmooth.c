
// Variables that hold the encoder values
float rightEncoder;
float leftEncoder;

// Holds the temporary sotrage for encoder readings
typedef struct {
  float EncoderStorage1;
  float EncoderStorage2;
} encoder;
encoder encR;
encoder encL;

// Generate the Average Encoder Values of REncoder1/2 & LEncoder1/2
void AverageEncoderValues(bool DualEncoder = true)
{
	// If you are using two encoders on each side
	if(DualEncoder == true)
	{
		// Collect Sensor Values
	  encR.EncoderStorage1 = SensorValue[rightEncoder1];
	  encR.EncoderStorage2 = SensorValue[rightEncoder2];
	  // Average the Sensor Values and save to rightEncoder
	  rightEncoder = (encR.EncoderStorage1 + encR.EncoderStorage2) / 2;

	  // Collect Sensor Values
	  encL.EncoderStorage1 = SensorValue[leftEncoder1];
	  encL.EncoderStorage2 = SensorValue[leftEncoder2];
	  // Average the Sensor Values and save to leftEncoder
	  leftEncoder = (encL.EncoderStorage1 + encL.EncoderStorage2) / 2;
	}
	// If you are onyl using one encoder on each side
	if(DualEncoder == false)
	{
		// Collect Sensor Values and write to rightEncoder
	  encR.EncoderStorage1 = SensorValue[rightEncoder1];
	  rightEncoder = encR.EncoderStorage1;

	  // Collect Sensor Values and write to leftEncoder
	  encL.EncoderStorage1 = SensorValue[leftEncoder1];
	  leftEncoder = encL.EncoderStorage1;
	}
}
