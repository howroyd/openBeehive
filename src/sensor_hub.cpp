#include <Arduino.h>
#include "sensor_hub.h"

Sensor_hub::Sensor_hub()
{
    ;
}

Sensor_hub::~Sensor_hub()
{
    delete tmp;
    delete lightMeter;
    delete mySensor;
    Wire.flush();
}

String Sensor_hub::getTmp(void)
{
    _newData = false;
    return _tmpVal;
}

String Sensor_hub::getLux(void)
{
    _newData = false;
    return _luxVal;
}

String Sensor_hub::getCo2(void)
{
    _newData = false;
    return _co2Val;
}

String Sensor_hub::getVoc(void)
{
    _newData = false;
    return _vocVal;
}

bool Sensor_hub::update(void)
{
    _update();
    return _newData;
}

bool Sensor_hub::update(const uint32_t period)
{
    static uint16_t tLast;

    if ((millis() - tLast) > period)
    {
        // Time to update!
        _update();
    }
    else
    {
        // Time to wait
        ;
    }
    return _newData;
}

bool Sensor_hub::_update(void)
{
    _tmpVal = String(tmp->readTemperature());
    _luxVal = String(lightMeter->readLightLevel());

    if (mySensor->dataAvailable())
    {
        //If so, have the sensor read and calculate the results.
        //Get them later
        mySensor->readAlgorithmResults();

        _co2Val = mySensor->getCO2();

        _vocVal = mySensor->getTVOC();
    }

    return _newData = true;
}

void Sensor_hub::begin(void)
{
    constructObjects();
    startWire();
    startSensors();
}

void Sensor_hub::begin(const uint8_t sda, const uint8_t scl)
{
    constructObjects();
    startWire(sda, scl);
    startSensors();
}

void Sensor_hub::startSensors(void)
{
    tmp->begin(_addrTmp);
    lightMeter->begin(BH1750::CONTINUOUS_LOW_RES_MODE);
    mySensor->begin();
}

void Sensor_hub::startWire(void)
{
    Wire.setClock(400000);
    Wire.begin(1, 3);
}

void Sensor_hub::startWire(const uint8_t sda, const uint8_t scl)
{
    Wire.setClock(400000UL);
    Wire.begin(sda, scl);
}

void Sensor_hub::setFreq(const uint32_t freq)
{
    _freq = freq;
}

void Sensor_hub::constructObjects(void)
{
    tmp = new Sensor_TMP102();
    lightMeter = new BH1750(_addrLux);
    mySensor = new CCS811(_addrCcs);
}

void Sensor_hub::setTmpAddr(const uint8_t addr)
{
    _addrTmp = addr;
}

void Sensor_hub::setLuxAddr(const uint8_t addr)
{
    _addrLux = addr;
}

void Sensor_hub::setCcsAddr(const uint8_t addr)
{
    _addrCcs = addr;
}

bool Sensor_hub::newData(void)
{
    return _newData;
}