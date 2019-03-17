#include "Configuration.h" //Configuring system parameters
#include "Definitions.h"   //Variable definitions
  
void setup() 
{ 
 hardwareSetup(); //Setting on board hardware such as timers and UART communication
}

void loop() 
{  
  timer_value = micros();
  
  if ((timer_value - timer_old) >= 1000) //1Khz loop (every millisec). Carries out packet routines, and position refresh routines
  {
    loop_counter++;
    timer_old = timer_value;
    
    robotStrategy(Networking());
    
    if ((loop_counter % 10) == 0)
      updatePosition_straight();
      
    positionControl();
  }
}
