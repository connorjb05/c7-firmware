#pragma once

#include <Arduino.h>

class ControlModule {
public:
    virtual void setup() = 0;
    virtual void loop() = 0;
    virtual void stop() = 0;

    virtual ~ControlModule() = default;
};