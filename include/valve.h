#ifndef SOLENOIDS_H
#define SOLENOIDS_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

class Valve
{
public:
    int pin;
    bool state = false;

    // Parameterised Constructor
    Valve(int pin)
    {
        this->pin = pin;
        gpio_set_dir(this->pin, GPIO_OUT);
        gpio_put(this->pin, 1);
    }

    void controlValve(bool);
};

#endif
