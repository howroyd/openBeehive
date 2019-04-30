#include <Arduino.h>
#include "sensor_hub.h"

Sensor_hub::Sensor_hub()
{
    ;
}

Sensor_hub::~Sensor_hub()
{
    delete tmp102;
    delete bh1750;
    delete ccs811;
    Wire.flush();
}

String Sensor_hub::getTicker(void)
{
    return String(_ticker);
}

String Sensor_hub::getTmp(void)
{
    _newData = false;
    return String(_tmpVal);
}

String Sensor_hub::getLux(void)
{
    _newData = false;
    return String(_luxVal);
}

String Sensor_hub::getCo2(void)
{
    _newData = false;
    return String(_co2Val);
}

String Sensor_hub::getVoc(void)
{
    _newData = false;
    return String(_vocVal);
}

String Sensor_hub::getCcsTmp(void)
{
    _newData = false;
    return String(_ccsTmp);
}

bool Sensor_hub::update(void)
{
    _update();
    return _newData;
}

bool Sensor_hub::update(const uint32_t period)
{
    static uint32_t tLast;

    if ((millis() - tLast) > period)
    {
        // Time to update!
        _update();
        tLast = millis();
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
    float newTmp = tmp102->readTemperature();
    if (newTmp > 0.0)
        _tmpVal = newTmp;

    float newLux = bh1750->readLightLevel(true); // true enables delay to take reading
    if (newLux > 0.0)
        _luxVal = newLux;

    //if (ccs811->dataAvailable())
    if (ccs811->available())
    {

        float newCcsTmp = (float)ccs811->calculateTemperature();
        if (newCcsTmp > 0.0)
            _ccsTmp = newCcsTmp;

        //If so, have the sensor read and calculate the results.
        //Get them later
        //ccs811->readAlgorithmResults();
        if (!ccs811->readData())
        {
            float newCo2 = (float)ccs811->geteCO2();
            //float newCo2 = (float)ccs811->getCO2();
            if (newCo2 > 0.0)
                _co2Val = newCo2;

            float newVoc = (float)ccs811->getTVOC();
            if (newVoc > 0.0)
                _vocVal = newVoc;
        }
        else
        {
            //_co2Val = -1.0F;
            //_vocVal = -1.0F;
        }
    }

    _ticker++;

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
    tmp102->begin(_addrTmp);
    bh1750->begin(BH1750::CONTINUOUS_LOW_RES_MODE);
    ccs811->begin(_addrCcs);
    calibrateCcs();
    //ccs811->begin();
    Wire.flush();
    update();
}

void Sensor_hub::calibrateCcs(void)
{
    while (!ccs811->available())
        ;
    float temp = ccs811->calculateTemperature();
    ccs811->setTempOffset(temp - 25.0);
}

void Sensor_hub::startWire(void)
{
    Wire.setClock(_freq);
    Wire.begin(1, 3);
}

void Sensor_hub::startWire(const uint8_t sda, const uint8_t scl)
{
    Wire.setClock(_freq);
    Wire.begin(sda, scl);
}

void Sensor_hub::setFreq(const uint32_t freq)
{
    _freq = freq;
}

void Sensor_hub::constructObjects(void)
{
    tmp102 = new Sensor_TMP102();
    bh1750 = new BH1750(_addrLux);
    ccs811 = new Adafruit_CCS811();
    //ccs811 = new CCS811(_addrCcs);
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