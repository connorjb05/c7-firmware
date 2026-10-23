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

    // Tracks the start of each shift so preShiftUs is saved only once.
    static bool wasShifting = false;

    // If the TCM is shifting
    // then ECU must free spin/coast
    if (tcm.isShifting() &&
        timestamp > 0
    ) 
    {

        if (!wasShifting) 
        {
            preShiftUs = currentUs;
            wasShifting = true;
        }

        targetUs = NEUTRAL_US;
        currentUs = NEUTRAL_US;

        writeToESC(currentUs);
        return;
    }

    wasShifting = false;

    /*
    TCM considers the shift as 'complete'.
    ECU will now provide power to the drive train
    */
    if (tcm.isShiftingCompleted() &&
        timestamp > 0
    ) 
    {
        
        VehicleSensors& sensors = Modules::getSensors();

        const float inputRPM = fabsf(sensors.getMotorRPM());
        const float outputRPM = fabsf(sensors.getOutputRPM());

        const uint8_t targetGearNum = static_cast<uint8_t>(
            tcm.getTargetGear()
        );

        const float targetGearRatio = VehicleConfig::Transmission

        tcm.setShiftingCompleted(false); // UPDATE THE TCM
        
        return; // Start throttle's acceleration AFTER the shift RPM sync
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