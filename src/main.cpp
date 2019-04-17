#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "uMQTTBroker.h"

uMQTTBroker myBroker;

void startWiFiClient();
void startWiFiAP();


/*
 * Your WiFi config here
 */
char ssid[] = "ssid";      // your network SSID (name)
char pass[] = "password"; // your network password



void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  // Connect to a WiFi network
  startWiFiClient();

  // Or start the ESP as AP
//startWiFiAP();

  // Start the broker
  Serial.println("Starting MQTT broker");
  myBroker.init();
}

void loop()
{   
  // do anything here
  delay(1000);
}

/*
 * WiFi init stuff
 */
void startWiFiClient()
{
  Serial.println("Connecting to "+(String)ssid);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  
  Serial.println("WiFi connected");
  Serial.println("IP address: " + WiFi.localIP().toString());
}

void startWiFiAP()
{
  WiFi.softAP(ssid, pass);
  Serial.println("AP started");
  Serial.println("IP address: " + WiFi.softAPIP().toString());
}
