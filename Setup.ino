void hardwareSetup()
{ 
  //M1 Pin assignment
  pinMode(A8,OUTPUT);  // ENABLE MOTOR PK0 A8
  pinMode(46,OUTPUT);  // STEP MOTOR PL3 D46
  pinMode(48,OUTPUT);  // DIR MOTOR PL1 D48
  //M2 Pin assignment
  pinMode(38,OUTPUT);  // ENABLE MOTOR PD7 D38
  pinMode(A0,OUTPUT);  // STEP MOTOR PF0 A0
  pinMode(A1,OUTPUT);  // DIR MOTOR PF1 A1

  //Initializing Debug Console
  Serial.begin(9600);
  delay(500);
  
  //Initializing UART communications with vision system
  Serial1.begin(1000000); //Baud rate is higher for improved response characteristics

  //Disable stepper motors untill timer hardware is enabled
  digitalWrite(A8,HIGH);
  digitalWrite(38,HIGH);
  
  //Enabling on-board timers for stepper pulse wave generation
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

  delay(500);
  //Enable Timer Interrupt Routines used to generate stepper pulse wave
  TIMSK1 |= (1<<OCIE1A);  // Enable Timer1 interrupt
  TIMSK3 |= (1<<OCIE1A);  // Enable Timer3 interrupt
  
  //Re-enable Steppers, all hardware is enabled
  digitalWrite(A8,LOW);
  digitalWrite(38,LOW);

  //Initializing Robot initial position and parameters...
  position_M1 =(ROBOT_INITIAL_POSITION_X + ROBOT_INITIAL_POSITION_Y) * AXIS_STEPS_PER_UNIT;
  position_M2 =(ROBOT_INITIAL_POSITION_X - ROBOT_INITIAL_POSITION_Y) * AXIS_STEPS_PER_UNIT;
  target_position_M1 = position_M1;
  target_position_M2 = position_M2;

  user_max_speed = MAX_SPEED;//Maximum speed and Acceleration are assinged as per configuration defenitions
  max_acceleration = MAX_ACCEL;
  max_speed = user_max_speed / 2;
  
  delay(500);

  timer_old = micros(); //Initializing program loop timer
  micros_old = timer_old;
}
