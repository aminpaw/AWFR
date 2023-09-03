#include "valve.h"

void Valve::controlValve(bool state)
{
    gpio_put(this->pin, state);
    this->state = state;
}