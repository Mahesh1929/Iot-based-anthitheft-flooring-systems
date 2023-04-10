#include <SPI.h>
#include <WiFi.h>

char mySSID[] = "iot123456";

char myPWD[] = "iot123456";
char myAPI[] = "9QNX5JDEY3Z0NX4A";  
const char* myHOST = "api.thingspeak.com";
String myPORT = "80";
const int analogInPin = 6;  
int sensorValue = 0;  
const unsigned long postingInterval = 3L * 1000L; 
WiFiClient client;


void setup() {
  Serial.begin(115200);         
  Serial.print("Attempting to connect to Network named: ");
  Serial.println(mySSID);
  pinMode(analogInPin,INPUT);
  
  WiFi.begin(mySSID, myPWD);
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print(".");         
    delay(300);
  }
}

void loop() {
     sensorValue = analogRead(analogInPin);            
  Serial.println(sensorValue);        
  delay(300);
  UploadData(0);
  if(sensorValue>2080){
       Serial.println("HIGH");                      
       Serial.println(sensorValue);
       UploadData(1);
       delay(postingInterval);
       UploadData(1);
       delay(postingInterval);
       UploadData(1);
       delay(postingInterval);
       UploadData(1);
       delay(postingInterval);
       delay(10000);          
  }

}



void UploadData(int data1){
Serial.print("connecting to ");
Serial.println(myHOST);

WiFiClient client;
const int httpPort = 80;
if (!client.connect(myHOST, httpPort)) {
  Serial.println("connection failed");
  return;
}

String sendData = "GET /update?api_key=";
sendData += "9QNX5JDEY3Z0NX4A";
sendData += "&field1=";
sendData += data1;
Serial.print("Requesting URL: ");
Serial.println(sendData);

  client.print(sendData + " HTTP/1.1\r\n" +
               "Host: " + myHOST + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
 
 
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
 
  Serial.println();
  Serial.println("closing connection");
}
