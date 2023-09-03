#ifndef LEVEL_SENSOR_H
#define LEVEL_SENSOR_H
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

class LevelSensor
{
public:
    int trigPin; // Trigger pin of the sensor
    int echoPin; // Echo pin of the sensor
    float level; // Level of the sensor
    float distance;

    // Default constructor
    LevelSensor()
    {
        trigPin = 0;
        echoPin = 0;
        level = 0;
    }

    // Parameterised Constructor
    LevelSensor(int trigPin, int echoPin)
    {
        this->trigPin = trigPin;
        gpio_set_dir(this->trigPin, GPIO_OUT);
        this->echoPin = echoPin;
        gpio_set_dir(this->echoPin, GPIO_IN);
        this->level = 0;
    }

    float readLevelSensor();
};

#endif