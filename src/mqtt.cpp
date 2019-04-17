#include <Arduino.h>
#include "uMQTTBroker.h"
#include "mqtt.h"

/*
 * Custom broker class with overwritten callback functions
 */

bool myMQTTBroker::onConnect(IPAddress addr, uint16_t client_count)
{
    Serial.println(addr.toString() + " connected");
    return true;
}

bool myMQTTBroker::onAuth(String username, String password)
{
    Serial.println("Username/Password: " + username + "/" + password);
    return true;
}

void myMQTTBroker::onData(String topic, const char *data, uint32_t length)
{
    char data_str[length + 1];
    os_memcpy(data_str, data, length);
    data_str[length] = '\0';

    Serial.println("received topic '" + topic + "' with data '" + (String)data_str + "'");
}
