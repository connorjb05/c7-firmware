#pragma once

#include <Arduino.h>
#include <ESP32Servo.h>
#include <cstdint>
#include <algorithm>

#include "DriveMode.h"
#include "Gear.h"
#include "ControlModule.h"
#include "TCM.h"
#include "VehicleConfig.h"

class ECU : public ControlModule
{

public:

    static constexpr uint16_t MIN_US = 1000;
    static constexpr uint16_t NEUTRAL_US = 1500;
    static constexpr uint16_t MAX_US = 2000;
    static constexpr uint16_t BRAKING_US = 1450;

    void setup() override;
    void loop() override;
    void stop() override;

    void setDriveMode(DriveMode mode);
    void setGear(Gear gear);
    void setThrottle(float throttle);
    void setBrake(bool brake);
    void setReversing(bool reversing);

    DriveMode getDriveMode() const;
    Gear getGear() const;
    float getThrottle() const;
    bool isBraking() const;
    bool isReversing() const;
    bool isNeutral() const;

private:

    Servo esc;

    DriveMode driveMode = static_cast<DriveMode>(0);
    Gear gear = static_cast<Gear>(0);

    float currentUs = NEUTRAL_US;
    float targetUs = NEUTRAL_US;
    float preShiftUs = NEUTRAL_US;

    float throttle = 0.0f;

    bool brake = false;
    bool reverse = false;

    float getAcceleration() const;

    void writeToESC(float f) {
        f = constrain(
            f, 
            static_cast<float>(MIN_US), 
            static_cast<float>(MAX_US)
        );

        esc.writeMicroseconds(
            static_cast<uint16_t>(f)
        );
    }

    float throttleToMicroseconds(float throttle) 
    {

        float microseconds = 0.0f;

        if (throttle >= 0.0f) 
        {
            microseconds = 
                NEUTRAL_US + 
                500 *
                throttle;
        } 
        else 
        {
            microseconds =
                NEUTRAL_US +
                throttle *
                (NEUTRAL_US - MIN_US);
        }

        return constrain(
            microseconds,
            static_cast<float>(MIN_US),
            static_cast<float>(MAX_US)
        );
    }
};
