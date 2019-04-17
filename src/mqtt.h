#pragma once

#include <Arduino.h>
#include "uMQTTBroker.h"

/*
 * Custom broker class with overwritten callback functions
 */
class myMQTTBroker : public uMQTTBroker
{
public:
    virtual bool onConnect(IPAddress addr, uint16_t client_count);

    virtual bool onAuth(String username, String password);

    virtual void onData(String topic, const char *data, uint32_t length);
};