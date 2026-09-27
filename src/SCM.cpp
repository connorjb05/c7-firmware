#include "SCM.h"
#include "Pins.h"

void SCM::setup()
{
    servo.setPeriodHertz(50);

    servo.attach(
        Pins::STEERING_SERVO
    );

    steering = 0.0f;

    servo.write(
        CENTER_ANGLE
    );
}

void SCM::loop()
{
    // No need rn, steering is applied instantly
    // in setSteering();
}

void SCM::stop()
{
    steering = 0.0f;

    servo.write(
        CENTER_ANGLE
    );
}

void SCM::setSteering(float steering) 
{
    steering = constrain(
        steering, 
        -1.0f, 
        1.0f
    );
    
    SCM::steering = steering;

    float angle;

    if (steering >= 0.0f) 
    {
        // Center -> Right

        angle = CENTER_ANGLE
            + steering
            * (RIGHT_ANGLE - CENTER_ANGLE);
    } 
    else 
    {
        // Center -> left

        angle = CENTER_ANGLE
            + (-steering)
            * (LEFT_ANGLE - CENTER_ANGLE);
    }

    servo.write(
        static_cast<int>(angle)
    );
}

