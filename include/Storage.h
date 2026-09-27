#pragma once

#include <Preferences.h>

#include "ControlModule.h"

class Storage : public ControlModule
{

public:

    void setup() override;
    void loop() override;
    void stop() override;

private:

    Preferences preference;

};