#include "StatusLED.h"

void StatusLED::setup()
{
    state = StatusLEDState::Booting;

    lastUpdatedMs = millis();

    setColor(
        0,
        0,
        BRIGHTNESS
    );
}

void StatusLED::loop() 
{
    switch (state)
    {

        case StatusLEDState::Booting:
        {
            setColor(
                0,
                0,
                BRIGHTNESS
            );

            break;
        }

        case StatusLEDState::WaitingConnection:
        {
            updateWaitingConnection();
            break;
        }

        case StatusLEDState::Connected:
        {
            setColor(
                0,
                BRIGHTNESS,
                0
            );

            break;
        }

        case StatusLEDState::ConnectionTimeout:
        {
            setColor(
                BRIGHTNESS,
                0,
                0
            );

            break;
        }

        case StatusLEDState::Error:
        {
            setColor(
                BRIGHTNESS,
                0,
                0
            );

            break;
        }

        case StatusLEDState::CriticalError:
        {
            updateCriticalError();
            break;
        }

        case StatusLEDState::SafeMode:
        {
            setColor(
                BRIGHTNESS,
                0,
                BRIGHTNESS
            );

            break;
        }

    }
}

void StatusLED::stop()
{
    setColor(
        0,
        0,
        0
    );
}

void StatusLED::updateWaitingConnection()
{
    uint32_t currentMs = millis();


    if (currentMs - lastUpdatedMs >= 700)
    {
        blinkState = !blinkState;

        lastUpdatedMs = currentMs;
    }


    if (blinkState)
    {
        setColor(
            BRIGHTNESS,
            BRIGHTNESS / 3,
            0
        );
    }
    else
    {
        setColor(
            0,
            0,
            0
        );
    }
}

void StatusLED::updateTimeout()
{
    uint32_t currentMs = millis();

    if (currentMs - lastUpdatedMs >= 250)
    {
        blinkState = !blinkState;

        lastUpdatedMs = currentMs;
    }

    if (blinkState)
    {
        setColor(
            BRIGHTNESS,
            BRIGHTNESS,
            0
        );
    }
    else
    {
        setColor(
            0,
            0,
            0
        );
    }
}

void StatusLED::updateCriticalError()
{
    uint32_t currentMs = millis();

    if (currentMs - lastUpdatedMs >= 100)
    {
        blinkState = !blinkState;

        lastUpdatedMs = currentMs;
    }


    if (blinkState)
    {
        setColor(
            BRIGHTNESS,
            0,
            0
        );
    }
    else
    {
        setColor(
            0,
            0,
            0
        );
    }
}

void StatusLED::setState(StatusLEDState state)
{
    if (StatusLED::state == state) 
    {
        return;
    }

    StatusLED::state = state;

    lastUpdatedMs = millis();

    blinkState = false;
}

StatusLEDState StatusLED::getState() const
{
    return state;
}

void StatusLED::setColor(
    uint8_t red,
    uint8_t green,
    uint8_t blue
) 
{
    neopixelWrite(
        Pins::STATUS_LED,
        red,
        green,
        blue
    );
}