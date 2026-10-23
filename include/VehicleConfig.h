#pragma once

#include <cstdint>
#include "Gear.h"

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

    namespace Battery 
    {
        inline constexpr uint8_t CELL_COUNT = 3;
        
        inline constexpr float NOMINAL_CELL_VOLTAGE = 3.7f;
        inline constexpr float MAX_CELL_VOLTAGE = 4.2f;

        inline constexpr uint16_t CAPACITY_MAH = 5200;

        inline constexpr float NOMINAL_VOLTAGE = CELL_COUNT * NOMINAL_CELL_VOLTAGE;

        inline constexpr float MAX_VOLTAGE = CELL_COUNT * MAX_CELL_VOLTAGE;
    }

    namespace Motor
    {
        inline constexpr uint16_t KV = 4000;
        inline constexpr float EFFICIENCY_MOTOR = 0.90f;
    
        inline constexpr float MAX_NO_LOAD_RPM = 
            static_cast<float>(KV) * Battery::MAX_VOLTAGE;

        inline constexpr float NOMINAL_NO_LOAD_RPM =
            static_cast<float>(KV) * Battery::NOMINAL_VOLTAGE;
    }

    namespace Transmission 
    {
        constexpr uint8_t GEAR_COUNT = 2;

        inline float getRatio(Gear gear) 
        {
            switch (gear) 
            {
                case Gear::First:
                    return 2.0f;
                    
                case Gear::Second:
                    return 0.8f;

                case Gear::Neutral:
                default:
                    return 0.0f;
            }
        }

        inline std::uint8_t getServoAngle(Gear gear) 
        {
            switch (gear) 
            {
                case Gear::First:
                    return 90;

                case Gear::Second:
                    return 90;

                case Gear::Neutral:
                default:
                    return 90;
            }
        }
    }

}