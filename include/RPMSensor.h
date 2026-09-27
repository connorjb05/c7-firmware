#pragma once

#include <Arduino.h>

class RPMSensor
{

public:

    RPMSensor(
        uint8_t pin,
        uint8_t pulsesPerRevolution = 1
    );

    void setup();
    void loop();

    float getRPM() const;

private:

    uint8_t pin;
    uint8_t pulsesPerRevolution;

    volatile uint32_t pulseCount = 0;

    uint32_t lastCalculationMs = 0;

    float rpm = 0.0f;

    static constexpr uint32_t SAMPLE_TIME_MS = 50;

    void IRAM_ATTR handlePulse();

    static void IRAM_ATTR interruptHandler(void* arg);
};