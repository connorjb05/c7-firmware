#include "TCM.h"
#include "Pins.h"
#include "Modules.h"

void TCM::setup()
{

    shifter.setPeriodHertz(50);
    
    shifter.attach(
        Pins::TRANSMISSION_SERVO
    );
    
    currentGear = static_cast<Gear>(0);
    targetGear = static_cast<Gear>(0);
}

void TCM::loop()
{

    ECU& ecu = Modules::getECU();

    uint32_t now = millis();

    if (TCM::shifting && 
        ecu.isNeutral() &&
        TCM::timeSinceLastShiftStarted <= 0
    ) 
    {

        TCM::timeSinceLastShiftStarted = now;

        uint8_t angle = getServoAngle(TCM::targetGear);
        shifter.write(angle);

    }

    if (now - TCM::timeSinceLastShiftStarted >= TCM::SHIFTING_TIME &&
        ecu.isNeutral() &&
        TCM::shifting
    ) 
    {
        TCM::shifting = false; // Tells the ECU that shifting has stopped/clutch is engaged
        TCM::timeSinceLastShiftStarted = 0; // Reset timer
        TCM::shiftComplete = true;
    }

}

void  TCM::stop()
{

}

void TCM::setTargetGear(Gear gear) 
{

    if (TCM::targetGear != gear) 
    {
        TCM::shifting = true;
    }

    TCM::targetGear = gear;
}

void TCM::setShiftingCompleted(bool completed)
{
    TCM::shiftComplete = completed;
}

Gear TCM::getCurrentGear() const
{
    return TCM::currentGear;
}

Gear TCM::getTargetGear() const
{
    return TCM::targetGear;
}

bool TCM::isShifting() const 
{
    return TCM::shifting;
}

bool TCM::isShiftingCompleted() const 
{
    return TCM::shiftComplete;
}
