# jubilant-octo-giggle

This was the repository for the final project of my Advanced Software Systems class, with two other team members. It is a simple IoT device that allows users to monitor and control the soil humidity of a plant through a webpage, using an arduino, a raspberry pi, a humidity sensor and a water pump. 

![alt text](https://github.com/sollozzo2/jubilant-octo-giggle/blob/master/rpi.PNG)
This picture was taken when we were still in progress.

Project: Soil Moisture and Watering System
Hardware used: Arduino Uno, relay, moisture sensor, 12v pump

Our server code is contained in server.c. Code to generate the histogram is in my-histogram.c. For serial communication with the Arduino, we used the pyserial library. A cgi script uses bash to call ser.py, which reads and writes to the Arduino, and writes to sensorval as an output for humidity readings. The value in sensorval is read and sent to the client by the cgi script. Code for the arduino is in py.ino.
The soil sensor is connected through an analog pin which gives a value between 200 and 700. The wetter the soil, the lower the value. A map function is used in the arduino sketch to convert the analog value into a percentage value between 0 and 100.

home.html is used as the interface for activating the sensor and the pump. There are two buttons, and each button leads to a .cgi script, which calls ser.py with the right argument.
