#pragma once

#include "ControlModule.h"
#include "RPMSensor.h"
#include "TempSensor.h"

#include "Pins.h"

class VehicleSensors : public ControlModule
{

public:

    VehicleSensors();

    void setup() override;
    void loop() override;
    void stop() override;

    float getMotorRPM() const;
    float getOutputRPM() const;
    
    float getMotorTemp() const;
    float getBatteryTemp() const;

private:

    RPMSensor motorRpmSensor;
    RPMSensor outputRpmSensor;

    TempSensor motorTempSensor;
    TempSensor batteryTempSensor;

};