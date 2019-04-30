#pragma once

#include <Arduino.h>
#include <Sensor_TMP102.h>
#include <BH1750.h>
#include "SparkFunCCS811.h"
#include "Adafruit_CCS811.h"

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

    String getTicker(void);

    String getTmp(void);
    String getLux(void);
    String getCo2(void);
    String getVoc(void);
    String getCcsTmp(void);

    void setTmpAddr(const uint8_t addr);
    void setLuxAddr(const uint8_t addr);
    void setCcsAddr(const uint8_t addr);

    void setFreq(const uint32_t freq);

protected:
    Sensor_TMP102 *tmp102;
    BH1750 *bh1750;
    Adafruit_CCS811 *ccs811;
    //CCS811 *ccs811;
    void startSensors(void);
    bool _update(void);
    void calibrateCcs(void);

private:
    void constructObjects(void);
    void startWire(void);
    void startWire(const uint8_t sda, const uint8_t scl);
    uint32_t _ticker;
    float _tmpVal = -0.1F;
    float _luxVal = -0.1F;
    float _co2Val = -0.1F;
    float _vocVal = -0.1F;
    float _ccsTmp = -0.1F;
    bool _newData = false;
    uint32_t _freq = 100000UL;
    uint8_t _addrTmp = 0x48;
    uint8_t _addrLux = 0x23;
    uint8_t _addrCcs = 0x5A;
};