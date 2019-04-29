#pragma once

#include <Arduino.h>
#include <PubSubClient.h>

#define MSG_BUF_SIZE 30

/*
 * Custom broker class with overwritten callback functions
 */
class MqttHandler : public PubSubClient
{
public:
    MqttHandler(Client &client);
    MqttHandler(const char *hostname, Client &client);

    void sendTele(const char *topic, const char *payload);

    void setHostname(const char *hostname);
    void setPeriod(const uint16_t period);

private:
    char _hostname[20]; // = "MqttHandler";
    uint16_t _period = 1000;
};