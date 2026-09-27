#include "RPMSensor.h"

RPMSensor::RPMSensor(
    uint8_t pin,
    uint8_t pulsesPerRevolution
) 
    : 
    pin(pin),
    pulsesPerRevolution(pulsesPerRevolution)
{
}

void RPMSensor::setup() 
{
    
    pinMode(
        pin,
        INPUT_PULLUP
    );

    attachInterruptArg(
        pin,
        interruptHandler,
        this,
        FALLING
    );

    lastCalculationMs = millis();
}

void RPMSensor::loop()
{
    uint32_t currentMs = millis();

    uint32_t elapsedMs = currentMs - lastCalculationMs;

    if (elapsedMs < SAMPLE_TIME_MS) 
    {
        return;
    }

    noInterrupts();

    uint32_t pulses = pulseCount;

    pulseCount = 0;

    interrupts();

    rpm = (static_cast<float>(pulses) / pulsesPerRevolution) 
        * (60000.0f / elapsedMs);
    
    lastCalculationMs = currentMs;
}

float RPMSensor::getRPM() const 
{
    return rpm;
}

void IRAM_ATTR RPMSensor::handlePulse() 
{
    pulseCount++;
}

void IRAM_ATTR RPMSensor::interruptHandler(void* arg)
{
    RPMSensor* sensor = static_cast<RPMSensor*>(arg);
    sensor->handlePulse();
}