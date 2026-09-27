#pragma once

class ECU;
class TCM;
class SCM;
class VehicleSensors;
class BluetoothController;
class StatusLED;
class Storage;

namespace Modules
{

    ECU& getECU();
    TCM& getTCM();
    SCM& getSCM();

    VehicleSensors& getSensors();
    BluetoothController& getBluetooth();
    StatusLED& getStatusLED();

    Storage& getStorage();

}