#include <ESP8266WiFi.h>


const int aqsensor = A0;  //output of mq135 connected to A0 pin of ESP-8266

String apiKey = "2S71FQ1BUUZYPW0O";     //  Enter your Write API key here
const char *ssid =  "KOS PAK AGUNG";     // Enter your WiFi Name
const char *pass =  "terserah"; // Enter your WiFi Password
const char* server = "api.thingspeak.com";
WiFiClient client;

void setup() {
  // put your setup code here, to run once:
  pinMode (aqsensor,INPUT); // MQ135 is connected as INPUT to ESP-8266
 
  Serial.begin (115200);      //begin serial communication with baud rate of 115200

  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP Address: ");
  delay(5000);
  Serial.println(WiFi.localIP());
  delay(5000);
}

void loop() {
  // put your main code here, to run repeatedly:

    int ppm = analogRead(aqsensor); //read MQ135 analog outputs at A0 and store it in ppm

  Serial.print("Air Quality: ");  //print message in serail monitor
  Serial.println(ppm);            //print value of ppm in serial monitor

  delay(1000);

  if (client.connect(server,80))   
                      {  
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(ppm);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
                             Serial.print("Air Quality: ");
                             Serial.print(ppm);
                             Serial.println(" PPM.Send to Thingspeak.");
                        }
  if(ppm <= 130)
  {
    Serial.println("AQ Level Normal");
  }
  else if (ppm > 130 && ppm < 250)
  {
    Serial.println("AQ Level Medium");
  }
  else
  {
    Serial.println("AQ Level Danger!");     
  }

          client.stop();
          Serial.println("Waiting...");
          delay(10000);

}
