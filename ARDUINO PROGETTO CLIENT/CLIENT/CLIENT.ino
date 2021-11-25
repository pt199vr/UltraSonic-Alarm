#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

//WIFI
const char* ssid = "YourWifiName";   // your network SSID (name) 
const char* password = "YourWifiPassword";   // your network password
int status = WL_IDLE_STATUS;  //status of the connection
byte server[] = {192,168,15,100}; //server ip address

#define trigPin 2 //attach pin GPIO-2 ESP8266 to pin Trig of HC-SR04
#define echoPin 0 //attach pin GPIO-0 ESP8266 to pin Echo of HC-SR04

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
int mean = 0;//variable for comparisons
int count = 0;//counter for printing only one time a command
bool sem = false; //variable for switching the system on or off

//variables for casting from int (distance) to char*
String distanceString = "";
char arrayDistance[10];


void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  Serial.println("Attempting to connect");
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {   
    delay(500);
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("\nConnected.");

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
}


void loop() {
  
     WiFiClient client;
     if (!client.connect(server, 4080)) {
      Serial.println("connection failed");
      delay(5000);
      return;
    }

    if(count == 0){
      Serial.println("Connected to the server");
      count++;
    }
    // Clears the trigPin condition
    digitalWrite(trigPin, LOW);
    delayMicroseconds(1000000);

    
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    
    // Calculating the distance
    distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
    
    if(mean == 0)
      mean = distance;
    else if(mean != 0 && !(distance <= mean - 10 || distance >= mean + 10))
      mean = (mean + distance) / 2;
      
    if(distance <= mean - 10 || distance >= mean + 10){
      Serial.print("WARNING !!!!!!!!!!!!!!!!!!!!! Distance: ");
      Serial.print(distance);
      Serial.println(" cm");
      
      
      client.write("WARNING !!!!!!!!!!!!!!!!!!!!! Distance: ");
      distanceString += distance;
      char mex[10]; 
      distanceString.toCharArray(mex, 10);
      client.write(mex);
      client.write(" cm");
      distanceString = "";
    }
    
    //Printing mean
    Serial.print("Mean: ");
    Serial.print(mean);
    Serial.println(" cm");
   
    ////////////////////////////////////////////////////////////
    // Displays the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
}
