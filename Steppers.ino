// TIMER 1 : STEPPER MOTOR SPEED CONTROL R
ISR(TIMER1_COMPA_vect) //Interupt handler with enabled interupts at Timer 1 Counter compare
{
  if (dir_M1==0) //if direction 0 do not move
    return;
  SET(PORTL,3); //Rising edge of the wave initiated
  position_M1 += dir_M1; //Step counter is incremented

  //Assembly instructions used to obtained desired duty cycle
  __asm__ __volatile__ (
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop");  // Wait 2 cycles. With the other instruction and this we ensure a more than 1 microsenconds step pulse
  CLR(PORTL,3); //Falling edge of the wave
}

// TIMER 3 : STEPPER MOTOR SPEED CONTROL L
ISR(TIMER3_COMPA_vect) //Same interrupt routine applied to the second stepepr motor, refer to subroutine TIMER1 for explanation
{
  if (dir_M2==0)
    return;
  SET(PORTF,0);
  position_M2 += dir_M2;
  
  __asm__ __volatile__ (
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop"); // Wait 2 cycles. With the other instruction and this we ensure a more than 1 microsenconds step pulse
  CLR(PORTF,0);
}


void positionControl() //Poistion control routine, executed once per 1KHz program loop
{

  int32_t temp;
  uint32_t timer;
  int16_t dt;
  int16_t result_speed_M1, result_speed_M2;
  int16_t absSpeedM1, absSpeedM2;
    
  real_position_x = (((float)position_M1 + (float)position_M2) / 2) / AXIS_STEPS_PER_UNIT; //Calculation of current X,Y position from step counters
  real_position_y = (((float)position_M1 - (float)position_M2) / 2) / AXIS_STEPS_PER_UNIT;
  
  timer = micros();
  dt = constrain(timer - micros_old, 0, 2000); //Delta time is around 1000 most of the time = 1 millisec. Time between positionControl executions
  micros_old = timer;
  
  acceleration_M1 = max_acceleration;
  acceleration_M2 = max_acceleration;

  absSpeedM1 = abs(speed_M1);
  absSpeedM2 = abs(speed_M2);

 if (absSpeedM1 < SCURVE_LOW_SPEED) {//Assignment of acceleration based on smooth S-profile
    acceleration_M1 = map(absSpeedM1, 0, SCURVE_LOW_SPEED, MIN_ACCEL, max_acceleration);
    if (acceleration_M1 > max_acceleration)
      acceleration_M1 = max_acceleration;
  }
  if (absSpeedM2 < SCURVE_LOW_SPEED) {//Similar routine repeated for stepper motor 2
    acceleration_M2 = map(absSpeedM2, 0, SCURVE_LOW_SPEED, MIN_ACCEL, max_acceleration);
    if (acceleration_M2 > max_acceleration)
      acceleration_M2 = max_acceleration;
  }
  // SCURVE FACOR 1900
 // MOTOR1 M1
  temp = (long)speed_M1 * speed_M1;
  temp = temp / (1900 * (long)acceleration_M1);
  pos_stop_M1 = position_M1 + sign(speed_M1) * temp;    // We calculate the stop position if we apply a deceleration right now. Current stepper distance + deceleration distance
  
  if (target_position_M1 > position_M1)    // Positive move
  {
    if (pos_stop_M1 >= target_position_M1)  // Do we need to start stopping?
      result_speed_M1 = 0;                    // The deceleration ramp is done inside the setSpeed function
    else
      result_speed_M1 = target_speed_M1;      // The aceleration ramp is done inside the setSpeed function
  }
  else   // Negative move
  {
    if (pos_stop_M1 <= target_position_M1)  // Start decelerating?
      result_speed_M1 = 0;
    else
      result_speed_M1 = -target_speed_M1;
  }

  // MOTOR2 M2 Same routine as for M1. Refer to M1 routine for comments
  temp = (long)speed_M2 * speed_M2;
  temp = temp / (1900 * (long)acceleration_M2);
  pos_stop_M2 = position_M2 + sign(speed_M2) * temp;
  
  if (target_position_M2 > position_M2) // Positive move
  {
    if (pos_stop_M2 >= target_position_M2)  // Start decelerating?
      result_speed_M2 = 0;                  // The deceleration ramp is done inside the setSpeed function
    else
      result_speed_M2 = target_speed_M2;    // The aceleration ramp is done inside the setSpeed function
  }
  else   // Negative move
  {
    if (pos_stop_M2 <= target_position_M2)  // Start decelerating?
      result_speed_M2 = 0;
    else
      result_speed_M2 = -target_speed_M2;
  }

  setMotorSpeed(result_speed_M1, result_speed_M2, dt); //Change motor speeds to desired as per positionControl routine
}

// Speed could be positive or negative
void setMotorSpeed(int16_t m1tspeed, int16_t m2tspeed, int16_t dt) //This function assinges on-board timer delays based on desired speed and switches movment directions if required.
{
  
  long timer_period; //Time between positionControl routine executions
  int16_t accel;

  // Limit max speeds
  m1tspeed = constrain(m1tspeed, -user_max_speed, user_max_speed);
  m2tspeed = constrain(m2tspeed, -user_max_speed, user_max_speed);

  // M1 MOTOR
  // We limit acceleration => speed ramp
  accel = ((long)acceleration_M1 * dt) / 1000; // Convertin predicted acceleration into speed increase in one positionControl iteration (potential speed change). We divide by 1000 because dt are in microseconds
  if (((long)m1tspeed - speed_M1) > accel) // Checking if desiered speed and current speed difference is greater than prediced speed increase. We use long here to avoid overflow on the operation
    speed_M1 += accel;
  else if (((long)speed_M1 - m1tspeed) > accel) //Similar to the first 'IF' statment. Checking if desired speed change can be acomplished.
    speed_M1 -= accel;
  else //We are already going close to the desired speed
    speed_M1 = m1tspeed;


  // Check if we need to change the direction pins
  if ((speed_M1 == 0) && (dir_M1 != 0))
    dir_M1 = 0; //This will stop the motor
  else if ((speed_M1 > 0) && (dir_M1 != 1))
  {
    CLR(PORTL, 1);
    dir_M1 = 1;
  }
  else if ((speed_M1 < 0) && (dir_M1 != -1))
  {
    SET(PORTL, 1);
    dir_M1 = -1;
  }

  if (speed_M1 == 0)
    timer_period = ZERO_SPEED; //This will stop the motor
  else if (speed_M1 > 0)
    timer_period = 2000000 / speed_M1; //Speed is converted into 2Mhz timer delay
  else
    timer_period = 2000000 / -speed_M1;

  if (timer_period > 65535)   // Correct overflow (maximun period without overflow)
    timer_period = ZERO_SPEED;

  OCR1A = timer_period;
  // Check  if we need to reset the timer...
  if (TCNT1 > OCR1A)
    TCNT1 = 0;

  // M2 MOTOR same routine as M1. Refer to comments on routine for M1
  accel = ((long)acceleration_M2 * dt) / 1000;
  if (((long)m2tspeed - speed_M2) > accel)
    speed_M2 += accel;
  else if (((long)speed_M2 - m2tspeed) > accel)
    speed_M2 -= accel;
  else
    speed_M2 = m2tspeed;

  // Check if we need to change the direction pins
  if ((speed_M2 == 0) && (dir_M2 != 0))
    dir_M2 = 0;
  else if ((speed_M2 > 0) && (dir_M2 != 1))
  {
    CLR(PORTF, 1);
    dir_M2 = 1;
  }
  else if ((speed_M2 < 0) && (dir_M2 != -1))
  {
    SET(PORTF, 1);
    dir_M2 = -1;
  }

  if (speed_M2 == 0)
    timer_period = ZERO_SPEED;
  else if (speed_M2 > 0)
    timer_period = 2000000 / speed_M2; // 2Mhz timer
  else
    timer_period = 2000000 / -speed_M2;

  if (timer_period > 65535)   // Check for minimun speed (maximun period without overflow)
    timer_period = ZERO_SPEED;

  OCR3A = timer_period;

  // Check  if we need to reset the timer...
  if (TCNT3 > OCR3A)
    TCNT3 = 0;  
}

void setPosition_straight(int target_x_mm_new, int target_y_mm_new)//New move routine, is called in network code when a move instruction packet is recieved
{
  int diff_M1; //Difference between target and current position in motor steps
  int diff_M2;
  float factor1; //velocity correction factor
  float factor2;
  long tspeed1;
  long tspeed2;
  long diffspeed1;
  long diffspeed2;
  float speedfactor1;
  float speedfactor2;

  target_x_mm = constrain(target_x_mm_new, ROBOT_MIN_X, ROBOT_MAX_X); //constrain coordinates to the bounds of table
  target_y_mm = constrain(target_y_mm_new, ROBOT_MIN_Y, ROBOT_MAX_Y);

  //if(target_y_mm < -273)  //Constrain cordinates at rounded ends of the table
    //target_x_mm = constrain(target_x_mm_new, -290, 290);
  if(target_y_mm < -341)
    target_x_mm = constrain(target_x_mm_new, -265, 265);
    
  target_position_M1 = ((float)target_x_mm + (float)target_y_mm) * AXIS_STEPS_PER_UNIT; //Convert table coordinate reference of frame to stepper reference of frame
  target_position_M2 = ((float)target_x_mm - (float)target_y_mm) * AXIS_STEPS_PER_UNIT;

  diff_M1 = myAbs(target_position_M1 - position_M1);//Difference between target position and current position
  diff_M2 = myAbs(target_position_M2 - position_M2);

  factor1 = 1.0; //corection factor applied to move in straight lines
  factor2 = 1.0;
  if (diff_M2 == 0) // to avoid division by 0
    factor2 = 0.0;
  else if (diff_M1 > diff_M2)
    factor2 = (float)diff_M2 / (float)diff_M1;
  else
    factor1 = (float)diff_M1 / (float)diff_M2;

  // Calculate the target speed (with sign) for each motor
  tspeed1 = sign(target_position_M1 - position_M1) * max_speed * factor1;
  tspeed2 = sign(target_position_M2 - position_M2) * max_speed * factor2;

  diffspeed1 = abs(speed_M1 - tspeed1);//Difference between current speed and target speed
  diffspeed2 = abs(speed_M2 - tspeed2);
    
  speedfactor1 = 1.05 - (diffspeed2 - diffspeed1) / (2.0 * max_speed); //Empirical speed factor
  speedfactor1 = constrain(speedfactor1, 0.0, 1.0);
  speedfactor2 = 1.05 - (diffspeed1 - diffspeed2) / (2.0 * max_speed);
  speedfactor2 = constrain(speedfactor2, 0.0, 1.0);

  target_speed_M1 = max_speed * factor1 * speedfactor1 * speedfactor1 ;
  target_speed_M2 = max_speed * factor2 * speedfactor2 * speedfactor2 ;
}

// Update speeds on each motor for straight line algorithm (diagonals)
void updatePosition_straight()
{
  int diff_M1;
  int diff_M2;
  float factor1;
  float factor2;
  long tspeed1;
  long tspeed2;
  long diffspeed1;
  long diffspeed2;
  float speedfactor1;
  float speedfactor2;

  // Speed adjust to draw straight lines (aproximation)
  diff_M1 = myAbs(target_position_M1 - position_M1);
  diff_M2 = myAbs(target_position_M2 - position_M2);

  // Now, we calculate the factor to apply to draw straight lines. Speed adjust based on target distance
  factor1 = 1.0;
  factor2 = 1.0;
  if (diff_M2 == 0) // to avoid division by 0
    factor2 = 0.0;
  else if (diff_M1 > diff_M2)
    factor2 = (float)diff_M2 / (float)diff_M1;
  else
    factor1 = (float)diff_M1 / (float)diff_M2;

  // Calculate the target speed (sign) for each motor
  tspeed1 = sign(target_position_M1 - position_M1) * max_speed * factor1;
  tspeed2 = sign(target_position_M2 - position_M2) * max_speed * factor2;
  
  // Now we calculate a compensation factor. This factor depends on the acceleration of each motor (difference on speed we need to apply to each motor)
  // This factor was empirically tested to reduce overshoots
  diffspeed1 = abs(speed_M1 - tspeed1);
  diffspeed2 = abs(speed_M2 - tspeed2);
  speedfactor1 = 1.05 - (diffspeed2 - diffspeed1) / (2.0 * max_speed);
  speedfactor1 = constrain(speedfactor1, 0.0, 1.0);
  speedfactor2 = 1.05 - (diffspeed1 - diffspeed2) / (2.0 * max_speed);
  speedfactor2 = constrain(speedfactor2, 0.0, 1.0);

  // Set motor speeds. We apply the straight factor and the "different acceleration" speedfactor (cuadratic application).
  target_speed_M1 = max_speed * factor1 * speedfactor1 * speedfactor1;
  target_speed_M2 = max_speed * factor2 * speedfactor2 * speedfactor2;
}
