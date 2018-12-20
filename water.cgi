#!/bin/bash
python3 ser.py 1

echo"Content-type: text/html\r\n\r\n"
echo "<!DOCTYPE html><html><body><h1>CS410 Plant System</h1><p>Humidity %:"
cat sensorval
echo "  Plant has been watered!"
echo "</p><button type=\"button\" value=\"Get Humidity\" title=\"Get Humidity\"onclick=\"location.href='/readsensor.cgi'\">Get Humidity</button><button type=\"button\" value=\"Water My Plant\" title=\"Water My Plant\"onclick=\"location.href='/water.cgi'\">Water My Plant</button></body></html>"

exit 0