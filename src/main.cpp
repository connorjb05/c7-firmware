#include <Arduino.h>

#include "Modules.h"

#include "ECU.h"
#include "TCM.h"
#include "SCM.h"

#include "StatusLED.h"
#include "BluetoothController.h"
#include "VehicleSensors.h"

void setup() {
    Serial.begin(115200);

    delay(1000);

    Serial.println("C7 Firmware starting...");

    BluetoothController& bluetooth = Modules::getBluetooth();
    StatusLED& statusLED = Modules::getStatusLED();
    VehicleSensors& sensors = Modules::getSensors();

    ECU& ecu = Modules::getECU();
    TCM& tcm = Modules::getTCM();
    SCM& scm = Modules::getSCM();

    statusLED.setup();

    statusLED.setup();
    statusLED.setState(
        StatusLEDState::Booting
    );

    bluetooth.setup();

    sensors.setup();

    ecu.setup();
    tcm.setup();
    scm.setup();

    statusLED.setState(
        StatusLEDState::WaitingConnection
    );
}

void loop() {

    BluetoothController& bluetooth = Modules::getBluetooth();
    VehicleSensors& sensors = Modules::getSensors();
    StatusLED& statusLED = Modules::getStatusLED();

    bluetooth.loop();

    sensors.loop();

    if (!bluetooth.isConnected()) 
    {
        statusLED.setState(
            StatusLEDState::WaitingConnection
        );
    }
    else if (bluetooth.hasTimedOut())
    {
        statusLED.setState(
            StatusLEDState::ConnectionTimeout
        );
    }
    else
    {
        statusLED.setState(
            StatusLEDState::Connected
        );
    }

    statusLED.loop();

    ECU& ecu = Modules::getECU();
    TCM& tcm = Modules::getTCM();
    SCM& scm = Modules::getSCM();

    /*
    Fail safe
    */
    if (bluetooth.hasTimedOut())
    {
        stop();
        return;
    }
    
    const ControllerInput& input = bluetooth.getInput();

    // ecu
    ecu.setThrottle(input.throttle);
    ecu.setDriveMode(input.driveMode);
    ecu.setGear(input.gear);
    ecu.setBrake(input.brake);

    // transmission
    tcm.setTargetGear(input.gear);

    // steering
    scm.setSteering(input.steering);

    // todo provide sensor data to modules
    // todo send sensor data to controller through telemetry

    // update modules
    tcm.loop();
    ecu.loop();
    scm.loop();
}

void stop() 
{
    Modules::getTCM().stop();
    Modules::getECU().stop();
    Modules::getSCM().stop();
    Modules::getBluetooth().stop();
    Modules::getSensors().stop();
    Modules::getStatusLED().stop();
}
