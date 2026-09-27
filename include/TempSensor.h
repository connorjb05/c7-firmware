#pragma once

#include <Arduino.h>

class TempSensor
{

public:

    explicit TempSensor(uint8_t pin);

    void setup();
    void loop();

    float getTemperatureC() const;

private:

    static constexpr uint32_t UPDATE_INTERVAL_MS = 100;

    uint8_t pin;

    float temperatureC = 0.0f;

    uint32_t lastUpdatedMs = 0;

    void updateTemperature();

};