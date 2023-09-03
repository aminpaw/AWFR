#include "controller.h"

void Controller::fillTank()
{
    // bool completeFlag = false;
    // this->status = fill_running;

    while (abs(tank->waterLevel - MAXLEVEL) > 1.0)
    {
        tank->inputValve->controlValve(true);
        tank->levelSensor->level += 0.5;

        vTaskDelay(pdMS_TO_TICKS(100));
    }

    tank->inputValve->controlValve(false);
    this->status = fill_complete;
}

void Controller::regulatePh()
{
    this->status = regulate_running;
    // TODO: Implement PID or bang bang controller that uses input water to increase ph and acid to decrease
    bool completeFlag = false;
    while (completeFlag == false)
    {
        tank->inputValve->controlValve(false);
        tank->outputValve->controlValve(false);
        tank->mixingValve->controlValve(true);
        tank->floodingPump->controlPump(true);

        while (abs(tank->avgPh - tank->phSetPoint) > 0.2)
        {
            int onTime = 0;
            tank->inputValve->controlValve(false);
            tank->outputValve->controlValve(false);
            tank->mixingValve->controlValve(true);

            tank->acidPump->controlPump(true);
            while (abs(tank->avgPh - tank->phSetPoint) > 0.2 && onTime < 2000)
            {
                onTime += 100;
                vTaskDelay(pdMS_TO_TICKS(100));
            }
            tank->acidPump->controlPump(false);
            vTaskDelay(pdMS_TO_TICKS(1000));
            tank->phSensor->reading -= 0.1;
        }

        vTaskDelay(pdMS_TO_TICKS(5000));
        tank->floodingPump->controlPump(false);
        vTaskDelay(pdMS_TO_TICKS(5000));

        tank->mixingValve->controlValve(false);

        if (abs(tank->avgPh - tank->phSetPoint) <= 0.2)
        {
            completeFlag = true;
        }
    }
    this->status = regulate_complete;
}

void Controller::floodWater()
{
    this->status = flood_running;

    tank->outputValve->controlValve(true);
    tank->floodingPump->controlPump(true);
    float floodTime = this->tank->floodTimeMin * 60 * 1000;
    while (floodTime > 0)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
        floodTime -= 1000;
        printf("REMANING TIME: %f ", floodTime);
    }
    tank->outputValve->controlValve(false);
    tank->floodingPump->controlPump(false);
    this->status = flood_complete;
}