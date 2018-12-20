import serial
import sys
from time import sleep
arduinoData = ""
toard = "b"
toard += "\'" + sys.argv[1] + "\'"
#print(toard)

ser = serial.Serial('/dev/ttyACM0', baudrate = 9600, timeout = 1)
#converting unicode to bytes so that the arduino can read it
toard = toard.encode('utf-8')
while arduinoData == "":
	ser.write(toard)
	arduinoData = ser.readline().decode('ascii')

print(arduinoData)

if sys.argv[1] == "2":
	myfile = open('sensorval', "w")
	myfile.write(arduinoData)

#bash script: python3 ser.py [1 or 2]
#takes 1 command line argument 
