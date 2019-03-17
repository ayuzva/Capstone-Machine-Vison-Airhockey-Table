import serial
ser = serial.Serial ("/dev/ttyS0", 1000000)    #Open port with baud rate
while True:
	ser.write("m")
			payload = (xcoord >> (8*0)) & 0xff
			ser.write(payload)
			payload = (xcoord >> (8*1)) & 0xff
			ser.write(payload)
			payload = (ycoord >> (8*0)) & 0xff
			ser.write(payload)
			payload = (ycoord >> (8*1)) & 0xff
			ser.write(payload)
	ser.write("m")
