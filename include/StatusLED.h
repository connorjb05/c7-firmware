#pragma once

#include <Arduino.h>

#include "ControlModule.h"
#include "Pins.h"

enum class StatusLEDState : uint8_t
{
    Booting,
    WaitingConnection,
    Connected,
    ConnectionTimeout,
    Error,
    CriticalError,
    SafeMode
};

class StatusLED : public ControlModule
{

public:

    void setup() override;
    void loop() override;
    void stop() override;

    void setState(StatusLEDState state);

    StatusLEDState getState() const;

private:

    static constexpr uint8_t BRIGHTNESS = 40;

    StatusLEDState state = StatusLEDState::Booting;

    uint32_t lastUpdatedMs = 0;

    bool blinkState = false;

    void setColor(
        uint8_t red,
        uint8_t green,
        uint8_t blue
    );

    void updateWaitingConnection();

    void updateTimeout();

    void updateCriticalError();

};