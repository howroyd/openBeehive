#include <Arduino.h>

#include "myWifi.h"
#include "myMqtt.h"

#include "myEsp8266Wifi.h"
#include "sensor_hub.h"
#include "mqtt.h"

WiFiClient espClient;
MqttHandler client(espClient);

Sensor_hub sensors;

void callback(char *topic, byte *payload, unsigned int length);
void publish(const unsigned long period);

char aliveTopic[] = "ALIVE";
unsigned long alive;

char tmpTopic[] = "TEMP";

char luxTopic[] = "LUX";

char co2Topic[] = "CO2";
char vocTopic[] = "VOC";
char ccsTmpTopic[] = "TMP2";

void setup()
{
  // Start WiFi
  startWiFiClient(wifiHome2G4.ssid, wifiHome2G4.pass);
  setArduinoOTA(wifiHome2G4.host);

  client.setServer(mqttOpenhab.server, mqttOpenhab.port);
  client.setCallback(callback);
  client.setHostname(wifiHome2G4.host);

  while (!client.connected())
  {
    if (!client.connect("ESP8266Client", mqttOpenhab.user, mqttOpenhab.pass))
      delay(2000);
  }

  //sensors.setFreq(400000UL);
  sensors.begin();
  sensors.update();
}

void loop()
{
  ArduinoOTA.poll();
  sensors.update(500UL);
  publish(2000UL);
}

void callback(char *topic, byte *payload, unsigned int length)
{
  ;
}

void publish(const unsigned long period_ms)
{
  static unsigned long tLast;
  if (((millis() - tLast) > period_ms) && sensors.newData())
  {
    client.sendTele(aliveTopic, String(alive++).c_str());
    client.sendTele(tmpTopic, sensors.getTmp().c_str());
    client.sendTele(luxTopic, sensors.getLux().c_str());
    client.sendTele(co2Topic, sensors.getCo2().c_str());
    client.sendTele(vocTopic, sensors.getVoc().c_str());
    client.sendTele(ccsTmpTopic, sensors.getCcsTmp().c_str());
    tLast = millis();
  }
}