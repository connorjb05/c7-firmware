#include "Modules.h"

#include "BluetoothController.h"
#include "VehicleSensors.h"
#include "StatusLED.h"

#include "ECU.h"
#include "TCM.h"
#include "SCM.h"

#include "Storage.h"

ECU ecu;
TCM tcm;
SCM scm;

VehicleSensors sensors;
BluetoothController bluetooth;
StatusLED statusLED;

Storage storage;

ECU& Modules::getECU()
{
    return ecu;
}

TCM& Modules::getTCM()
{
    return tcm;
}

SCM& Modules::getSCM()
{
    return scm;
}

VehicleSensors& Modules::getSensors()
{
    return sensors;
}

BluetoothController& Modules::getBluetooth()
{
    return bluetooth;
}

StatusLED& Modules::getStatusLED() 
{
    return statusLED;
}

Storage& Modules::getStorage()
{
    return storage;
}