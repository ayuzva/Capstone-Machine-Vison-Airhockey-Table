#include <stdint.h>

long loop_counter;
long timer_old;
long timer_packet_old;
long timer_value;
uint32_t micros_old;

volatile int16_t position_M1;  // This variables are modified inside the Timer interrupts
volatile int16_t position_M2;

int16_t real_position_x;
int16_t real_position_y;



//Stepper directions also needed in interrupts can be substituted for NOPs
int8_t dir_M1;     //(dir=1 positive, dir=-1 negative)
int8_t dir_M2;

int16_t speed_M1;
int16_t speed_M2;
int16_t target_position_M1;
int16_t target_position_M2;
int16_t target_speed_M1;
int16_t target_speed_M2;
int16_t acceleration_M1;
int16_t acceleration_M2;
int16_t max_speed;
int16_t max_acceleration;

int16_t pos_stop_M1;
int16_t pos_stop_M2;


uint16_t target_x_mm;
uint16_t target_y_mm;

//check these
int16_t user_max_speed;
int16_t user_max_accel;

unsigned int xcoord;
unsigned int ycoord;

// Some util functions...
int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

//uint8_t newPacket;
char Buffer[1]; //= "00";

// Arduino abs function sometimes fail!
int16_t myAbs(int16_t param)
{
  if (param < 0)
    return -param;
  else
    return param;
}

// Extract sign of a variable
int sign(int val)
{
  if (val < 0)
    return (-1);
  else
    return (1);
}
