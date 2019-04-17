#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "uMQTTBroker.h"
#include "myWifi.h"

/*
 * Your WiFi config here
 */
char* ssid = wifiHome.ssid;     // your network SSID (name)
char* pass = wifiHome.pass;     // your network password

void startWiFiClient();
void startWiFiAP();
bool WiFiAP = false; // Do yo want the ESP as AP?

/*
 * Custom broker class with overwritten callback functions
 */
class myMQTTBroker : public uMQTTBroker
{
public:
  virtual bool onConnect(IPAddress addr, uint16_t client_count)
  {
    Serial.println(addr.toString() + " connected");
    return true;
  }

  virtual bool onAuth(String username, String password)
  {
    Serial.println("Username/Password: " + username + "/" + password);
    return true;
  }

  virtual void onData(String topic, const char *data, uint32_t length)
  {
    char data_str[length + 1];
    os_memcpy(data_str, data, length);
    data_str[length] = '\0';

    Serial.println("received topic '" + topic + "' with data '" + (String)data_str + "'");
  }
};

myMQTTBroker myBroker;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  // Start WiFi
  if (WiFiAP)
    startWiFiAP();
  else
    startWiFiClient();

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

/*
 * WiFi init stuff
 */
void startWiFiClient()
{
  Serial.println("Connecting to " + (String)ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
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