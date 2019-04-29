
#include <Arduino.h>
#include <ESP8266WiFi.h>
//#include <ESP8266mDNS.h>
//#include <WiFiUdp.h>
#include <ArduinoOTA.h>


#include "myWifi.h"

#pragma once

/*
 * WiFi init stuff
 */
void startWiFiClient(const char *ssid, const char *pass)
{
  //Serial.println("Connecting to " + (String)ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    //Serial.print(".");
  }
  //Serial.println("");

  //Serial.println("WiFi connected");
  //Serial.println("IP address: " + WiFi.localIP().toString());
}

void setArduinoOTA(const char *hostname)
{
  // start the WiFi OTA library with internal (flash) based storage
  ArduinoOTA.begin(WiFi.localIP(), "Arduino", "password", InternalStorage);
}

void startWiFiAP(const char *ssid, const char *pass)
{
  WiFi.softAP(ssid, pass);
  //Serial.println("AP started");
  //Serial.println("IP address: " + WiFi.softAPIP().toString());
}