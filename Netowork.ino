char Networking()//Function that is called in the main program loop, it handles instruction recieved from vision processing system 
{
  if(Serial1.available() > 0) //Check if UART connection is still establshed
  {
    packetRead();//Call to coordinate packet handling routine
    
    if(Buffer[0]== 'm' && Buffer[1]== 'n') return 'a'; //Packet type for assigning desired position of the robot(Defence)
    else if(Buffer[0]== 'a' && Buffer[1]== 'k') return 'b'; //straight line move
    else if(Buffer[0]== 'c' && Buffer[1]== 'l') return 'c';//Packet to calibrate the system due to missed steps
    else if(Buffer[0]== 'f' && Buffer[1]== 'p') return 'f'; //Packet to calibrate the system initially 
    else if(Buffer[0]== 'r' && Buffer[1]== 't') return 'd';
    else if(Buffer[0]== 'l' && Buffer[1]== 't') return 'e';
    else return 'z';
  }else
  {
    return 'z';
  }
}

void packetRead() //Coordinate handling routine consisting of 6 8-bit packets
{
    Buffer[0] = Serial1.read(); //Starting byte used to check for completness of the packet
    xcoord=Serial1.read(); //Routine for X 16 bit unsigned integer X coordinate
    xcoord+=Serial1.read() << 8;
    ycoord=Serial1.read(); //Routine for Y 16 bit unsigned integer X coordinate
    ycoord+=Serial1.read() << 8;
    Buffer[1] = Serial1.read(); //Ending byte used to check for completness of the packet

    xcoords=xcoord-374;//Conversion from vision system coordinate frame to robot coordinate frame. Offset between 0,0 of both coordinate systems
    ycoords=ycoord-415;
}
