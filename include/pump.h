#ifndef PUMPS_H
#define PUMPS_H

#include <hardware/gpio.h>
#include <stdio.h>
#include <pico/stdlib.h>

// Pump class to represent
// a node of the linked list.
class Pump
{
public:
    int pin;
    bool state = false;

    // Default constructor
    Pump()
    {
        pin = 0;
    }

    // Parameterised Constructor
    Pump(int pin)
    {
        this->pin = pin;
        gpio_set_dir(this->pin, GPIO_OUT);
        gpio_put(this->pin, 1);
    }

    void controlPump(bool);
};

#endif
