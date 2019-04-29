#include <Arduino.h>
#include "myEsp8266Wifi.h"
#include "myWifi.h"
#include "myMqtt.h"
#include <PubSubClient.h>
#include <Sensor_TMP102.h>
#include <BH1750.h>
#include "Adafruit_CCS811.h"
#include "ClosedCube_HDC1080.h"
#include "SparkFunCCS811.h"

WiFiClient espClient;
PubSubClient client(espClient);
//PubSubClient client(&WiFi);

void callback(char *topic, byte *payload, unsigned int length);
void publish(int);

char aliveTopic[30];
unsigned long alive;

Sensor_TMP102 tmp;
char tmpTopic[30];
String tmpVal;

BH1750 lightMeter(0x23);
char luxTopic[30];
String lux;

//ClosedCube_HDC1080 hdc1080;
//Adafruit_CCS811 ccs;
CCS811 mySensor(0x5a);
char co2Topic[30];
String co2;
char vocTopic[30];
String voc;
char tmp2Topic[30];
String tmp2;

void setup()
{
  Wire.setClock(400000);
  Wire.begin(1, 3); // Using Rx & Tx pins
  //Wire.begin(0, 2); // Using GPIO 0 (SDA) & GPIO 2 (SCL)

  // Start WiFi
  startWiFiClient(wifiHome2G4.ssid, wifiHome2G4.pass);
  setArduinoOTA(wifiHome2G4.host);

  client.setServer(mqttOpenhab.server, mqttOpenhab.port);
  client.setCallback(callback);

  while (!client.connected())
  {
    if (!client.connect("ESP8266Client", mqttOpenhab.user, mqttOpenhab.pass))
      delay(2000);
  }

  strcpy(aliveTopic, "tele/");
  strcat(aliveTopic, wifiHome2G4.host);
  strcat(aliveTopic, "/ALIVE");

  tmp.begin(0x48);
  strcpy(tmpTopic, "tele/");
  strcat(tmpTopic, wifiHome2G4.host);
  strcat(tmpTopic, "/TEMP");

  lightMeter.begin(BH1750::CONTINUOUS_LOW_RES_MODE);
  strcpy(luxTopic, "tele/");
  strcat(luxTopic, wifiHome2G4.host);
  strcat(luxTopic, "/LUX");

  //hdc1080.begin(0x40);
  //hdc1080.setResolution(HDC1080_RESOLUTION_14BIT, HDC1080_RESOLUTION_14BIT);
  //ccs.begin(0x5a);
  //while(!ccs.available());
  //ccs.setTempOffset(ccs.calculateTemperature() - 25.0);
  mySensor.begin();
  strcpy(co2Topic, "tele/");
  strcat(co2Topic, wifiHome2G4.host);
  strcat(co2Topic, "/CO2");

  strcpy(vocTopic, "tele/");
  strcat(vocTopic, wifiHome2G4.host);
  strcat(vocTopic, "/VOC");

  strcpy(tmp2Topic, "tele/");
  strcat(tmp2Topic, wifiHome2G4.host);
  strcat(tmp2Topic, "/TMP2");
}

int myDelay = 100;

void loop()
{
  ArduinoOTA.poll();

  publish(2000);

  tmpVal = String(tmp.readTemperature());

  lux = String(lightMeter.readLightLevel());

  if (mySensor.dataAvailable())
  {
    //If so, have the sensor read and calculate the results.
    //Get them later
    mySensor.readAlgorithmResults();

    co2 = mySensor.getCO2();

    voc = mySensor.getTVOC();
  }

  /*
  if (ccs.available())
  {
    if (!ccs.readData())
    {
      co2 = ccs.geteCO2();

      voc = ccs.getTVOC();

      tmp2 = ccs.calculateTemperature();
    }
    else
    {
      ;
    }
  }*/

  // wait a second
  delay(myDelay);
}

void callback(char *topic, byte *payload, unsigned int length)
{
  ;
}

void publish(int period_ms)
{
  static unsigned long tLast;
  if ((millis() - tLast) > period_ms)
  {
    client.publish(aliveTopic, String(alive++).c_str());
    client.publish(tmpTopic, tmpVal.c_str());
    client.publish(luxTopic, lux.c_str());
    client.publish(co2Topic, String(co2).c_str());
    client.publish(vocTopic, String(voc).c_str());
    client.publish(tmp2Topic, String(tmp2).c_str());
    tLast = millis();
  }
}