#include "TempSensor.h"

TempSensor::TempSensor(uint8_t pin) 
    : pin(pin)
{
}

void TempSensor::setup()
{
    pinMode(pin, INPUT);

    analogReadResolution(12);

    updateTemperature();

    lastUpdatedMs = millis();
}

void TempSensor::loop()
{
    uint32_t currentMs = millis();

    if (currentMs - lastUpdatedMs < UPDATE_INTERVAL_MS) 
    {
        return;
    }

    updateTemperature();

    lastUpdatedMs = millis();
}

void TempSensor::updateTemperature() 
{
    uint32_t voltageMv = analogReadMilliVolts(pin);

    // TMP36:
    // 500mV = 0 C
    // +10mV = +1 C

    temperatureC = (static_cast<float>(voltageMv) - 500.0f) / 10.0f;
}

float TempSensor::getTemperatureC() const
{
    return temperatureC;
}