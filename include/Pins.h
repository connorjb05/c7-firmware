#pragma once

#include <Arduino.h>

namespace Pins
{

    static constexpr uint8_t STATUS_LED = 48;

    static constexpr uint8_t ESC = 17;
    static constexpr uint8_t STEERING_SERVO = 18;
    static constexpr uint8_t TRANSMISSION_SERVO = 19;

    static constexpr uint8_t MOTOR_RPM = 4;
    static constexpr uint8_t OUTPUT_RPM = 5;
    static constexpr uint8_t BATTERY_TEMP = 6;
    static constexpr uint8_t MOTOR_TEMP = 7;

}