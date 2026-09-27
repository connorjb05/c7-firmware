#pragma once

#include <Arduino.h>
#include <ESP32Servo.h>
#include <cstdint>

#include "ControlModule.h"

class SCM : public ControlModule
{

public:
    void setup() override;
    void loop() override;
    void stop() override;

    void setSteering(float steering);
    
private:
    static constexpr uint8_t LEFT_ANGLE = 50;
    static constexpr uint8_t CENTER_ANGLE = 50;
    static constexpr uint8_t RIGHT_ANGLE = 50;
    // TODO angle calibration!!

    Servo servo;

    float steering = 0.0f;
};