#pragma once

#include <Arduino.h>
#include <ESP32Servo.h>
#include <cstdint>

#include "ControlModule.h"
#include "Gear.h"
#include "ECU.h"

class TCM : public ControlModule
{

public:

    void setup() override;
    void loop() override;
    void stop() override;

    void setTargetGear(Gear gear);
    void setShiftingCompleted(bool completed);

    Gear getCurrentGear() const;
    Gear getTargetGear() const;

    bool isShifting() const;
    bool isShiftingCompleted() const;

private:

    static constexpr uint32_t SHIFTING_TIME = 50; // in millis

    Servo shifter;

    Gear currentGear = static_cast<Gear>(0);
    Gear targetGear = static_cast<Gear>(0);

    bool shifting = false;
    bool shiftComplete = false;

    uint32_t timeSinceLastShiftStarted = 0;

};