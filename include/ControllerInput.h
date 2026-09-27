#pragma once

#include <cstdint>

#include "DriveMode.h"
#include "Gear.h"

struct ControllerInput
{
    float throttle = 0.0f;
    float steering = 0.0f;

    Gear gear = static_cast<Gear>(0);
    DriveMode driveMode = static_cast<DriveMode>(0);

    bool lights = false;
    bool brake = false;

    uint8_t sequence = 0;
    uint32_t receivedAtMs = 0;
};