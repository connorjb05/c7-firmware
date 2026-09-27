#include "ECU.h"
#include "Pins.h"
#include "Modules.h"
#include "VehicleSensors.h"

void ECU::setup() 
{
    esc.setPeriodHertz(50);

    esc.attach(
        Pins::ESC,
        ECU::MIN_US,
        ECU::MAX_US
    );

    currentUs = ECU::NEUTRAL_US;
    targetUs = ECU::NEUTRAL_US;

    brake = false;
    reverse = false;

    ECU::writeToESC(currentUs);
}

void ECU::loop()
{

    TCM& tcm = Modules::getTCM();

    uint32_t timestamp = millis();

    // If the TCM is shifting
    // then ECU must free spin/coast
    if (tcm.isShifting() &&
        timestamp > 0
    ) 
    {
        // Keep track of currentUs before shifting
        // for mathematical estimation for after the shift
        preShiftUs = currentUs;

        targetUs = NEUTRAL_US;
        currentUs = targetUs;
        ECU::writeToESC(currentUs);
        return;
    }

    /*
    TCM considers the shift as 'complete'.
    ECU will now provide power to the drive train
    */
    if (!tcm.isShifting() &&
        tcm.isShiftingCompleted() &&
        ECU::isNeutral() &&
        timestamp > 0
    ) 
    {
        /*
        apply power to the drive train
        
        There's two ways to do this:
        
        First: Using sensors to match the input rpm 
        to the output rpm, then depending on throttle
        apply acceleration. 
        
        Second: Using an mathematics to estimate
        what the input rpm should be based on the what the
        rpm was when starting the shift.
        */

        VehicleSensors& sensors = Modules::getSensors();

        if (sensors.getOutputRPM() > 0) // RPM matching
        {

            float maxRPM = 11.1f * 4000.0f * 0.97f;
            float targetRPM = sensors.getOutputRPM();

            float targetUs = NEUTRAL_US + 
                (targetRPM / maxRPM) * 
                (MAX_US - NEUTRAL_US);

            ECU::currentUs = min(
                currentUs + getAcceleration(),
                targetUs
            );

        }
        else // Mathematical estimation
        {
            float acceleration = getAcceleration();
            
            ECU::targetUs = preShiftUs;
            ECU::currentUs = min(
                currentUs + acceleration,
                targetUs
            );
        }

        ECU::writeToESC(currentUs);

        tcm.setShiftingCompleted(false); // UPDATE THE TCM
        
        return; // Start acceleration based on throttle AFTER the shift RPM sync
    }
    
    if (brake) 
    {
        targetUs = BRAKING_US;

        constexpr float factor = 0.002f;
        
        currentUs += (targetUs - currentUs) * factor;
    } 
    else 
    {
        float acceleration = getAcceleration();
        
        currentUs = min(
            currentUs + acceleration, 
            targetUs
        );
    }

    ECU::writeToESC(currentUs);
}

void ECU::stop()
{
    // TODO, ensure there's neutral after
    // braking is over

    throttle = 0.0f;
    brake = true;
}

void ECU::setDriveMode(DriveMode driveMode) 
{
    ECU::driveMode = driveMode;
}

void ECU::setGear(Gear gear) 
{
    ECU::gear = gear;
}

void ECU::setThrottle(float throttle) 
{
    ECU::throttle = constrain(throttle, -1.0f, 1.0f);
    targetUs = ECU::throttleToMicroseconds(throttle);
}

void ECU::setBrake(bool brake)
{
    ECU::brake = brake;
}

void ECU::setReversing(bool reversing)
{
    ECU::reverse = reversing;
}

float ECU::getAcceleration() const
{
    return 1.80556 * throttle * throttle * throttle
                -
                0.70833 * throttle * throttle
                + 0.15278 * throttle;
}

DriveMode ECU::getDriveMode() const 
{
    return driveMode;
}

Gear ECU::getGear() const 
{
    return gear;
}

float ECU::getThrottle() const 
{
    return throttle;
}

bool ECU::isBraking() const
{
    return brake;
}

bool ECU::isReversing() const
{
    return reverse;
}

bool ECU::isNeutral() const
{
    return ECU::currentUs == ECU::NEUTRAL_US;
}