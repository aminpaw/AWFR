#include "pump.h"

void Pump::controlPump(bool action)
{
    gpio_put(this->pin, state);
    this->state = state;
}