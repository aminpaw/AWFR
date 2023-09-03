#ifndef TANK_H
#define TANK_H

#include "ezo_sensor.h"
#include "level_sensor.h"
#include "pump.h"
#include "valve.h"
#include "temp_sensor.h"

typedef enum ErrorCode
{
    success = 1,
    ph_syntax_error = 21,
    ph_no_data = 2541,
    ph_not_ready = 2551,
    ec_syntax_error = 22,
    ec_no_data = 2542,
    ec_not_ready = 2552,

} ErrorCode;

class Tank
{
public:
    EzoSensor *phSensor;
    EzoSensor *ecSensor;
    LevelSensor *levelSensor;
    TempSensor *tempSensor;
    Pump *floodingPump;
    Pump *drainingPump;
    Pump *acidPump;
    Pump *fertilizerPump;
    Valve *mixingValve;
    Valve *inputValve;
    Valve *outputValve;

    float avgPh = 1.2;
    float avgEc;
    float waterLevel = 12.0;
    float waterTemp;

    float phSetPoint = 7;
    float ecSetPoint = 1;
    float floodTimeMin = 1;
    float drainTimeMin = 1;

    // Parameterised Constructor
    Tank(EzoSensor *phSensor,
         EzoSensor *ecSensor,
         LevelSensor *levelSensor,
         TempSensor *tempSensor,
         Pump *floodingPump,
         Pump *drainingPump,
         Pump *acidPump,
         Pump *fertilizerPump,
         Valve *mixingValve,
         Valve *inputValve,
         Valve *outputValve)
    {
        this->phSensor = phSensor;
        this->ecSensor = ecSensor;
        this->levelSensor = levelSensor;
        this->tempSensor = tempSensor;
        this->floodingPump = floodingPump;
        this->drainingPump = drainingPump;
        this->acidPump = acidPump;
        this->inputValve = inputValve;
        this->mixingValve = mixingValve;
        this->outputValve = outputValve;
    }

    ErrorCode readPhEc();
    ErrorCode readPh();
    ErrorCode readEc();
    ErrorCode readLevel();
    ErrorCode readTemp();
};

#endif