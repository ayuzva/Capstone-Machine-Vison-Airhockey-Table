// ABSOLUTE MAX ROBOT SPEED AND ACCELERATION
// THIS VALUES DEPENDS ON YOUR ROBOT CONSTRUCTION (MOTORS, MECHANICS...)
// RECOMMENDED VALUES FOR 12V POWER SUPPLY
#define MAX_ACCEL 275           // Maximun motor acceleration in (steps/seg2)/1000. Max recommended value:280
#define MAX_SPEED 32000         // Maximun speed in steps/seg. Max absolute value: 32767!!

#define MIN_ACCEL 100
#define MIN_SPEED 5000
#define SCURVE_LOW_SPEED 2500
#define SCURVE_HIGH_SPEED 28000

// Geometric calibration.
// This depends on the pulley teeth. DEFAULT: 200(steps/rev)*8(microstepping) = 1600 steps/rev. 1600/32teeth*2mm(GT2) = 25 steps/mm
#define X_AXIS_STEPS_PER_UNIT 25    
#define Y_AXIS_STEPS_PER_UNIT 25

// This is the center of the table. All units in milimeters
// This are the dimensions of the official air hockey table Exploter from Buffalo
#define TABLE_LENGTH 895
#define TABLE_WIDTH 435
#define ROBOT_CENTER_X TABLE_WIDTH/2   // Center of robot.
#define ROBOT_CENTER_Y TABLE_LENGTH/2

// Absolute Min and Max robot positions in mm (measured from center of robot pusher)
#define ROBOT_MIN_X 60
#define ROBOT_MIN_Y 58
#define ROBOT_MAX_X TABLE_WIDTH-ROBOT_MIN_X
#define ROBOT_MAX_Y ROBOT_CENTER_Y-80

// Initial robot position in mm
// The robot must be at this position at start time
// Default: Centered in X and minimun position in Y
#define ROBOT_INITIAL_POSITION_X 0
#define ROBOT_INITIAL_POSITION_Y 0

// Utils (don´t modify)
#define CLR(x,y) (x&=(~(1<<y)))
#define SET(x,y) (x|=(1<<y)) //X ORED with y shifted by 1 to the left

//Zero Speed 1111111111111111 written to OCR1A really large value
#define ZERO_SPEED 65535
