// This is a simple func/define thing that stops the drive motors from buzzing :)
// Because they are annonying when they buzz :)
#define DEADBAND_THRESHOLD 10.0
#define DEADBAND(x) (abs(x) >= DEADBAND_THRESHOLD ? x : 0)
