// ABSOLUTE MAX ROBOT SPEED AND ACCELERATION
//FOR 24V POWER SUPPLY and 1/2 microstepping
#define MAX_ACCEL 115//115//140 //Maximun motor acceleration in (steps/sec2)/1000. Max recommended value:140 (steps/sec2)/1000 with 1/2 micro
#define MAX_SPEED 14500//16384 //Maximun speed in steps/sec.

#define MIN_ACCEL 50  //Bounds of acceleration profile 
#define MIN_SPEED 2500 //Bounds of speed profile
#define SCURVE_LOW_SPEED 1250
#define SCURVE_HIGH_SPEED 12200//14000

// Geometric calibration.
// This depends on the pulley teeth. 200(steps/rev)*2(microstepping) = 400 steps/rev. 400/32teeth*2mm(GT2) = 6.25 steps/mm
#define AXIS_STEPS_PER_UNIT 6.25

// Absolute Min and Max robot positions in mm (measured from center robot playing field)//374+x 415+y
#define ROBOT_MIN_X -310// 
#define ROBOT_MIN_Y -375//-355//
#define ROBOT_MAX_X 311// 685
#define ROBOT_MAX_Y 369//

// Initial robot position in mm
// The robot must be at this position at start time, unless calibration is applied.
// Default: Centered in X and Y
#define ROBOT_INITIAL_POSITION_X 0
#define ROBOT_INITIAL_POSITION_Y 0

// Utilities to enable and disable on board components
#define CLR(x,y) (x&=(~(1<<y)))
#define SET(x,y) (x|=(1<<y)) //X ORED with y shifted by 1 to the left

// Zero Speed 1111111111111111 written to OCR1A to disable stepper pulse timer
#define ZERO_SPEED 65535
