//OCT 2018
//Configurations and Variables

//Both files WIP
#include "Configuration.h"
#include "Definitions.h"   // Variable definitions
  
void setup() 
{ 
  //PIN Definition 
  // STEPPER MOTOR PINS (SAME AS RAMPS 1.4)
  // MOTOR R
  //     Z-STEP: D46    
  //     Z-DIR:  D48    
  //     Z-ENABLE: A8 
  // MOTOR L
  //     X-STEP: A0    
  //     X-DIR:  A1    
  //     X-ENABLE: D38  

  //M1
  pinMode(A8,OUTPUT);  // ENABLE MOTOR PK0
  pinMode(46,OUTPUT);  // STEP MOTOR PL3
  pinMode(48,OUTPUT);  // DIR MOTOR PL1
  //M2
  pinMode(38,OUTPUT);  // ENABLE MOTOR PD7
  pinMode(A0,OUTPUT);  // STEP MOTOR PF0
  pinMode(A1,OUTPUT);  // DIR MOTOR PF1

  //Init console output
  Serial.begin(9600);
  //Serial.println("Hello, Starting Init V0.1");
  delay(2000);

  //UART init
  Serial1.begin(1000000); //start UART @ baud, wanna go as fast as possible 1mill for stable connection

  //Stepper init
  //Disable Motors
  digitalWrite(A8,HIGH);
  digitalWrite(38,HIGH);
  
  //TIMER ENABLE FOR STEPEPRS Clear Timer on Compare Match
  // TIMER1 CTC MODE
  TCCR1B &= ~(1<<WGM13);
  TCCR1B |=  (1<<WGM12);
  TCCR1A &= ~(1<<WGM11); 
  TCCR1A &= ~(1<<WGM10);

   // output mode = 00 (disconnected)
  TCCR1A &= ~(3<<COM1A0); 
  TCCR1A &= ~(3<<COM1B0); 

  // Set the timer pre-scaler
  // Generally we use a divider of 8, resulting in a 2MHz timer on 16MHz CPU
  TCCR1B = (TCCR1B & ~(0x07<<CS10)) | (2<<CS10);

  OCR1A = ZERO_SPEED;   // Motor stopped
  dir_M1 = 0; //will not move
  TCNT1 = 0;
  
  // TIMER3 CTC MODE
  TCCR3B &= ~(1<<WGM13);
  TCCR3B |=  (1<<WGM12);
  TCCR3A &= ~(1<<WGM11); 
  TCCR3A &= ~(1<<WGM10);

  // output mode = 00 (disconnected)
  TCCR3A &= ~(3<<COM1A0); 
  TCCR3A &= ~(3<<COM1B0); 

  // Set the timer pre-scaler
  // Generally we use a divider of 8, resulting in a 2MHz timer on 16MHz CPU
  TCCR3B = (TCCR3B & ~(0x07<<CS10)) | (2<<CS10);

  OCR3A = ZERO_SPEED;   // Motor stopped
  dir_M2 = 0;
  TCNT3 = 0;

  delay(1000);
  //Enable Timer Interrupt---------------------------------------------------------------------------
  TIMSK1 |= (1<<OCIE1A);  // Enable Timer1 interrupt
  TIMSK3 |= (1<<OCIE1A);  // Enable Timer3 interrupt
  
  //Re-enable Steppers
  digitalWrite(A8,LOW);
  digitalWrite(38,LOW);

  //Initializing Robot initial position and parameters...
  position_M1 =(ROBOT_INITIAL_POSITION_X + ROBOT_INITIAL_POSITION_Y) * X_AXIS_STEPS_PER_UNIT;
  position_M2 =(ROBOT_INITIAL_POSITION_X - ROBOT_INITIAL_POSITION_Y) * Y_AXIS_STEPS_PER_UNIT;
  target_position_M1 = position_M1;
  target_position_M2 = position_M2;

  user_max_speed = MAX_SPEED;
  user_max_accel = MAX_ACCEL;
  max_acceleration = user_max_accel;
  max_speed = user_max_speed / 2;
  
  delay(4000);
  //Serial.println("Init done");
  delay(2000);

  timer_old = micros();
  timer_packet_old = timer_old;
  micros_old = timer_old;

  setPosition_straight(550,0);
}

void loop() 
{  

  //packetRead();
  
  timer_value = micros();
  
  if ((timer_value - timer_old) >= 1000) // MAIN 1Khz loop (every millisec)
  {
    timer_old = timer_value;
    loop_counter++;
        
    if ((loop_counter % 10) == 0)
    updatePosition_straight();
    
    positionControl();
  }
}
