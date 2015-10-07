
// Pie for everyone
#define PI 3.141592653589
// The Flywheel Gear Ratio
#define fw_GearRatio 17

// Set to Constants after tuning
// K denotes a PID constant
float  kp = 0;
float  ki = 0;
float  kd = 0;

// The target speed in RPMS of the flywheel wheel, set in main block
float targetVelocityRPM;

// The zone in which the I part of PID will activate
// Calculated in RPM
float integralActiveZone = 1000;

// Wait time for calculation
float velocityCalculationWaitTime = 20;

// Structure the hold all the flywheel related variables
typedef struct _flywheelPID
{
  // Current error
  float error;
  // Error Total
  float errorT;
  // Last iterations error
  float lastError;
  // the result of a proportion calculation
  float proportion;
  // the result of a integral calculation
  float integral;
  // the result of a derivative calculation
  float derivative;
  // The Current Velocity ( Measured in RPM )
  float velocity;

  // D.T.A Code
  // The Motor speed we want to achieve
  float TargetMotorSpeed;
  // Our Current Motor Speed
  float CurrentMotorSpeed;
} flywheel;
flywheel fwR;
flywheel fwL;

// D.T.A System Code:
void fw_UpdateMotorSpeed()
{
  // Update Current Motor Speed
  fwR.CurrentMotorSpeed = fwR.TargetMotorSpeed;
  fwL.CurrentMotorSpeed = fwL.TargetMotorSpeed;

  // Update Left FLywheel Motors
  motor[leftf1] = fwL.CurrentMotorSpeed;
  motor[leftf2] = fwL.CurrentMotorSpeed;

  // Update Right FLywheel Motors
  motor[rightf1] = fwR.CurrentMotorSpeed;
  motor[rightf2] = fwR.CurrentMotorSpeed;
}

// Velocity = result2 - result1 / waitTime * 1000 * 60 * Gear Ratio
// Returns the RPM of the wheel on a flywheel
float velocityCalculator(int result1, int result2)
{
  return ( ((result2 - result1) / velocityCalculationWaitTime * 1000) * 60 * fw_GearRatio);
}

// Calculates Velocity and updates the flywheel velocity variables
void fw_CalculateVelocity()
{
  // Temporary SensorValue Storage ( velocity measure 1/2 Left/Right)
  float velocityM1L = SensorValue[rightEncoder];
  float velocityM1R = SensorValue[leftEncoder];
  wait1Msec( velocityCalculationWaitTime );
  float velocityM2L = SensorValue[rightEncoder];
  float velocityM2R = SensorValue[leftEncoder];

  // Calucluates Velocity ( Measured in RPM )
  fwR.velocity = velocityCalculator(velocityM1R, velocityM2R);
  fwL.velocity = velocityCalculator(velocityM1L, velocityM2L);
}

task fw_PIDController()
{

  // Clears encoders
  SensorValue[ rightEncoder ] = 0;
  SensorValue[ leftEncoder ] = 0;

  while(true)
  {

    // Calculates this iterations velocity
    fw_CalculateVelocity();

    // Calculates error as the Target - the current
    fwR.error = targetVelocityRPM - fwR.velocity;
    fwL.error = targetVelocityRPM - fwL.velocity;

    // Building up of integral error on Left Side
    // Only if intergral Active Zone is true
    if( fwL.error < integralActiveZone && fwL.error != 0 )
    {
      // If the Integral part is active add the current ERROR to total error
      fwL.errorT += fwL.error;
    }
    else
    {
      // Otherwise the error should be 0
      fwL.errorT;
    }

    // Building up of integral error on Right Side
    // Only if intergral Active Zone is true
    if( fwR.error < integralActiveZone && fwR.error )
    {
      // If the Integral part is active add the current ERROR to total error
      fwR.errorT += fwR.error;
    }
    else
    {
      // Otherwise the error should be 0
      fwR.errorT = 0;
    }

    // Restrict the Ki Value to stop things getting out of hand
    if( fwL.errorT > 50/ki )
    {
      fwL.errorT = 50/ki;
    }
    if( fwR.errorT  > 50/ki )
    {
      fwR.errorT = 50/ki;
    }

    // If the error is not existant, dont feed the derivitave :)
    if( fwL.error == 0 )
    {
      fwL.derivative = 0;
    }
    if( fwR.error == 0 )
    {
      fwR.derivative = 0;
    }

    /* Set PID values                                         */

    // Linear Proportion of the error
    fwL.proportion      =     fwL.error                   * kp;
    fwR.proportion      =     fwR.error                   * kp;
    // The accumulated error (errorT) * ki
    fwL.integral        =     fwL.errorT                  * ki;
    fwR.integral        =     fwR.errorT                  * ki;
    // The rate of change of error * kd
    fwL.derivative      =     (fwL.error - fwL.lastError) * kd;
    fwR.derivative      =     (fwR.error - fwR.lastError) * kd;

    // For derivative
    fwL.lastError = fwL.error;
    fwR.lastError = fwR.error;


    /* Motor Speed Ajustment                                  */

    // Calculate the motor speed from the results of P.I.D
    // You add them together to get an appropriate speed
    fwR.TargetMotorSpeed = fwR.proportion + fwR.integral + fwR.derivative;
    fwL.TargetMotorSpeed = fwL.proportion + fwL.integral + fwL.derivative;

    // Makes sure the flywheel dosnt go backwards :)
    if(fwR.CurrentMotorSpeed < 0)
    {
      fwR.CurrentMotorSpeed = 0;
    }
    if(fwL.CurrentMotorSpeed < 0)
    {
      fwL.CurrentMotorSpeed = 0;
    }

    // D.T.A Sets FLywheel Speed
    fw_UpdateMotorSpeed();


  }

}
