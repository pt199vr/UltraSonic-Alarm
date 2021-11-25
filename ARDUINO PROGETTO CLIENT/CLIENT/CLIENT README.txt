Like in the Server we have used ESP8266WiFi.h library.

From line 4 to 7 you can find Wifi parameters for the connection.
Be sure to have written the same ip for server[] here and staticIp in SERVER.ino file.


trigPin and echoPin are two pins for the ultrasonic sensor.

The SETUP part is moreless the SERVER SETUP so you can read there what it does. 

In the loop part the CLIENT continue to check the connection between it and the SERVER.
Then it compute the the distance from the duration time.

We have thought to choose a threshold for the possible changes in the distance calculated.
This is 10 cm from a mean value.
Mean valuse is an integer which it is updated every loop to have a realistic mean distance.

 If the value computed is out of this threshold the CLIENT sends a warning message to the server.
