#pragma once

#include <cstdint>

namespace VehicleConfig 
{


    namespace Vehicle
    {
        inline constexpr float MASS_KG = 6.0f;
        inline constexpr float WHEELBASE_MM = 255.0f;
    }

    namespace Tires
    {
        inline constexpr float DIAMETER_MM = 65.0f;

        inline constexpr float CIRCUMFERENCE_MM = DIAMETER_MM * PI;
    }

    namespace Differential
    {
        inline constexpr float GEAR_RATIO = 2.6f;
    }

    namespace Motor
    {
        inline constexpr uint16_t KV = 4000;
        inline constexpr float EFFICIENCY_MOTOR = 0.90f;
    }

    namespace Battery 
    {
        inline constexpr uint8_t CELL_COUNT = 3;
        inline constexpr float NOMINAL_CELL_VOLTAGE = 3.7f;
        inline constexpr uint16_t CAPACITY_MAH = 5200;

        inline constexpr float NOMINAL_VOLTAGE = CELL_COUNT * NOMINAL_CELL_VOLTAGE;
    }

    namespace Transmission
    {
        inline constexpr uint8_t GEAR_COUNT = 2;

        // placeholder ratios
        inline constexpr float GEAR_RATIOS[GEAR_COUNT] = {
            2.0f, // First
            0.8f, // Second
        };

        inline constexpr float getRatio(uint8_t gear) 
        {
            return (gear >= 1 && gear <= GEAR_COUNT) 
            ? GEAR_RATIOS[gear - 1]
            : 0.0f;
        }
    }

    namespace ESC
    {
        inline constexpr uint16_t MIN_US = 1000;
        inline constexpr uint16_t NEUTRAL_US = 1500;
        inline constexpr uint16_t MAX_US = 2000;

        inline constexpr uint16_t PWM_FREQUENCY_HZ = 50;
    }

}