#include "VehicleSensors.h"

VehicleSensors::VehicleSensors() :
    motorRpmSensor(
        Pins::MOTOR_RPM,
        1
    ),

    outputRpmSensor(
        Pins::OUTPUT_RPM,
        1
    ),

    motorTempSensor(
        Pins::MOTOR_TEMP
    ),

    batteryTempSensor(
        Pins::BATTERY_TEMP
    )
{    
}

void VehicleSensors::setup() 
{
    motorRpmSensor.setup();
    outputRpmSensor.setup();

    motorTempSensor.setup();
    batteryTempSensor.setup();
}

void VehicleSensors::loop()
{
    motorRpmSensor.loop();
    outputRpmSensor.loop();

    motorTempSensor.loop();
    batteryTempSensor.loop();
}

// Ignore stop for sensors, unnecessary
void VehicleSensors::stop() 
{
}

float VehicleSensors::getMotorRPM() const
{
    return motorRpmSensor.getRPM();
}

float VehicleSensors::getOutputRPM() const
{
    return outputRpmSensor.getRPM();
}

float VehicleSensors::getMotorTemp() const
{
    return motorTempSensor.getTemperatureC();
}

float VehicleSensors::getBatteryTemp() const
{
    return batteryTempSensor.getTemperatureC();
}
