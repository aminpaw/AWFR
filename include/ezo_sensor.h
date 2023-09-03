#ifndef EZO_SENSOR_H
#define EZO_SENSOR_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include <string>
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
using namespace std;

typedef enum SensorType
{
    Undeclared = 0,
    PhSensor = 10,
    EcSensor = 11
} SensorType;
// Sensor class to represent
// a node of the linked list.
int reg_read(i2c_inst_t *i2c,
             const uint addr,
             uint8_t *buf,
             const uint8_t nbytes);

int reg_write(i2c_inst_t *i2c,
              const uint addr,
              uint8_t *buf,
              const uint8_t nbytes);

class EzoSensor
{
public:
    SensorType sensorType;
    int busID;
    string responseErrorStr = "NO DATA";
    int responseError = 254;
    int responseDelayMS = 900;
    float reading = 7.0;

    // Default constructor
    EzoSensor()
    {
        this->sensorType = Undeclared;
        this->busID = 00;
    }

    // Parameterised Constructor
    EzoSensor(SensorType sensorType)
    {
        this->sensorType = sensorType;
        this->busID = (int)sensorType;
    }

    EzoSensor(SensorType sensorType, int busID)
    {
        this->sensorType = sensorType;
        this->busID = busID;
    }

    float readEzoSensor();
    void sendReadCommand();
    float receiveReading();
};

#endif