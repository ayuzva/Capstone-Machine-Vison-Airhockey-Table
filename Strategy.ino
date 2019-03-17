void robotStrategy(char machineState){
  switch(machineState)
  {
    case'a'://general_move
      if((myAbs(real_position_x-xcoords) > 5 && myAbs(real_position_y-ycoords) > 5) && (myAbs(target_x_mm-xcoords) > 5 || myAbs(target_y_mm-ycoords) > 5))
      { //if target FAR and NEW target
        Serial.print("mov /n");
        max_acceleration = 115;//Gotta reset at some point
        setPosition_straight(xcoords, ycoords); 
      }
      break;
    case'b'://straight move (attack move)
      Serial.print("atk /n");
      if((myAbs(target_x_mm - xcoords) > 5))//add limit bypass
      {
        max_acceleration = 60; //Can probably increase
        setPosition_straight(xcoords, real_position_y);
      }
      else//else if not already moving forward ???
      {
        setPosition_straight(xcoords, ycoords);
      }
      break;
    case'c'://calibrate
      if(myAbs(speed_M1) < 100 && myAbs(speed_M2) < 100 && myAbs(xcoords) < 310 && ycoords < 369 && (myAbs(real_position_x-xcoords) > 10 && myAbs(real_position_y-ycoords) > 10))//Constraints under which calibration is applied, subject to change with testing
      {
        Serial.print("cal /n");
        position_M1 = ((float)xcoords + (float)ycoords) * AXIS_STEPS_PER_UNIT; //Reassigning current position of the robot to calibrated position
        position_M2 = ((float)xcoords - (float)ycoords) * AXIS_STEPS_PER_UNIT;
      }
      break;
    case'd'://right avoid
      if((myAbs(real_position_x - 290) > 5))
      {
        setPosition_straight(290, real_position_y);
      }
      else
      {
        setPosition_straight(290, -341);
      }
      break;
    case'e'://left avoid
      if((myAbs(real_position_x - 290) > 5))
      {
        setPosition_straight(-290, real_position_y);
      }
      else
      {
        setPosition_straight(-290, -341);
      }
      break;
    case'f'://first latch
      Serial.print("init /n");
      position_M1 = ((float)xcoords + (float)ycoords) * AXIS_STEPS_PER_UNIT; //Reassigning current position of the robot to calibrated position
      position_M2 = ((float)xcoords - (float)ycoords) * AXIS_STEPS_PER_UNIT;
      target_position_M1 = position_M1; //Reassigning target position of the robot to same position
      target_position_M2 = position_M2;
      break;
    case'z':
      //do nothing
      break;
    default:
    Serial.print("ERROR");
      break;
  }
}
