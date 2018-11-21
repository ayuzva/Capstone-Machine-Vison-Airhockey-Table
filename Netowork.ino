void packetRead()
{

    if(Serial1.available() > 0) {
      Buffer[0] = Serial1.read();
      xcoord=Serial1.read();
      xcoord+=Serial1.read() << 8;
      
      ycoord=Serial1.read();
      ycoord+=Serial1.read() << 8;
      Buffer[1] = Serial1.read(); 

      if(Buffer[0]== 'm' && Buffer[1]== 'm')
      {  
        /*Serial.print(xcoord);
        Serial.print(",");
        Serial.print(ycoord);*/
        setPosition_straight(xcoord, ycoord);
      }
    }
}
