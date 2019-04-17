#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "myWifi.h"

#pragma once

/*
 * WiFi init stuff
 */
void startWiFiClient(const char* ssid, const char* pass)
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

void startWiFiAP(const char* ssid, const char* pass)
{
  WiFi.softAP(ssid, pass);
  Serial.println("AP started");
  Serial.println("IP address: " + WiFi.softAPIP().toString());
}