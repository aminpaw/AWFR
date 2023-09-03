#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H
// #include <parameters.h>
#include "parameters.h"
#include <hardware/gpio.h>
#include <hardware/timer.h>
#include "pico/time.h"
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "one_wire.h"

class TempSensor
{
public:
    int pin;                 // Trigger pin of the sensor
    rom_address_t address{}; // Address of the sensor
    float temparature;       // Temparature read from the sensor
    One_wire *wire;          // One wire object

    // Parameterised Constructor
    TempSensor(int pin)
    {
        this->pin = pin;
        this->wire = new One_wire(pin);
        wire->init();
    }

    float readTempSensor();
};

#endif