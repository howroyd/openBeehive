#include <Arduino.h>
#include "myWifi.h"
#include "myEsp8266Wifi.h"
#include "mqtt.h"

char *ssid = wifiHome2G4.ssid; // your network SSID (name)
char *pass = wifiHome2G4.pass; // your network password

bool WiFiAP = false; // Do yo want the ESP as AP?

myMQTTBroker myBroker;

void setup()
{
  Serial.begin(115200);

  // Start WiFi
  if (WiFiAP)
    startWiFiAP(ssid, pass);
  else
    startWiFiClient(ssid, pass);

  // Start the broker
  Serial.println("Starting MQTT broker");
  myBroker.init();

  /*
 * Subscribe to anything
 */
  myBroker.subscribe("#");
}

int counter = 0;

void loop()
{
  /*
 * Publish the counter value as String
 */
  myBroker.publish("broker/counter", (String)counter++);

  // wait a second
  delay(1000);
}