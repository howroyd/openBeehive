#pragma once

#include <Arduino.h>
#include <Sensor_TMP102.h>
#include <BH1750.h>
#include "SparkFunCCS811.h"

class Sensor_hub
{
public:
    Sensor_hub(void);
    ~Sensor_hub(void);
    void begin(void);
    void begin(const uint8_t sda, const uint8_t scl);
    bool update(void);
    bool update(const uint32_t period);

    bool newData(void);

    String getTmp(void);
    String getLux(void);
    String getCo2(void);
    String getVoc(void);

    void setTmpAddr(const uint8_t addr);
    void setLuxAddr(const uint8_t addr);
    void setCcsAddr(const uint8_t addr);

    void setFreq(const uint32_t freq);

protected:
    Sensor_TMP102 *tmp;
    BH1750 *lightMeter;
    CCS811 *mySensor;
    void startSensors(void);
    bool _update(void);

private:
    void constructObjects(void);
    void startWire(void);
    void startWire(const uint8_t sda, const uint8_t scl);
    String _tmpVal;
    String _luxVal;
    String _co2Val;
    String _vocVal;
    bool _newData = false;
    uint32_t _freq = 100000UL;
    uint8_t _addrTmp = 0x48;
    uint8_t _addrLux = 0x23;
    uint8_t _addrCcs = 0x5A;
};