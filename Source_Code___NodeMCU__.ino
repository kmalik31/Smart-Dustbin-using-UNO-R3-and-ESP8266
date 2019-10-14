#include <ESP8266WiFi.h>

String deviceId = "xxxxxxxxxxxxxxxxxxxx";
const char* logServer = "api.pushingbox.com", ssid = "xxxxxxxxxxxxx", password = "xxxxxxxxx";

const int trigPin = D1, echoPin = D0;
long duration = 0;
int distance = 0;

void setup()
{
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  Serial.begin(115200); 
}

void sendNotification(String message)
{
  Serial.println("- connecting to Home Router SSID: " + String(ssid));
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("- succesfully connected");
  Serial.println("- starting client");
  
  WiFiClient client;

  Serial.println("- connecting to pushing server: " + String(logServer));
  if(client.connect(logServer, 80))
  {
    Serial.println("- succesfully connected");
    
    String postStr = "devid=";
    postStr += String(deviceId);
    postStr += "&message_parameter=";
    postStr += String(message);
    postStr += "\r\n\r\n";
    
    Serial.println("- sending data...");
    
    client.print("POST /pushingbox HTTP/1.1\n");
    client.print("Host: api.pushingbox.com\n");
    client.print("Connection: close\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
  }
  client.stop();
  Serial.println("- stopping the client");
}

void loop()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * (0.034 / 2);
  Serial.print("Distance: ");
  Serial.println(distance);
  if(distance < = 10)
  {
    sendNotification("..........Done.......");
    Serial.println("Working Sucessfully");
  }
}
