#include <Arduino.h>
#include "mqtt.h"

/*
 * Custom broker class with overwritten callback functions
 */
MqttHandler::MqttHandler(Client &client) : PubSubClient::PubSubClient(client)
{
    setHostname("MqttHandler");
}

MqttHandler::MqttHandler(const char *hostname, Client &client) : PubSubClient::PubSubClient(client)
{
    setHostname(hostname);
}

void MqttHandler::sendTele(const char *topic, const char *payload)
{
    static char *_buf = (char *)calloc(MSG_BUF_SIZE, sizeof(char));
    strcpy(_buf, "tele/");
    strcat(_buf, _hostname);
    strcat(_buf, "/");
    strcat(_buf, topic);
    publish(_buf, payload);
}

void MqttHandler::setHostname(const char *hostname)
{
    strcpy(_hostname, hostname);
}

void MqttHandler::setPeriod(const uint16_t period)
{
    _period = period;
}