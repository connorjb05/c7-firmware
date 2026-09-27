#pragma once

#include <cstdint>

enum class Gear : uint8_t
{
    Neutral = 0,
    First = 1,
    Second = 2
};

// TODO ensure these angle values
// are calibrated
static std::uint8_t getServoAngle(Gear gear) 
{
    switch (gear) 
    {
        case Gear::Neutral:
            return 90;
        case Gear::First:
            return 90;
        case Gear::Second:
            return 90;
    }

    return 90;
}