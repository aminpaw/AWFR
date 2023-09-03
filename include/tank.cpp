#include "tank.h"

ErrorCode Tank::readPhEc()
{
    ErrorCode error = success;
    this->phSensor->sendReadCommand();
    vTaskDelay(pdMS_TO_TICKS(100));
    this->ecSensor->sendReadCommand();
    vTaskDelay(pdMS_TO_TICKS(this->phSensor->responseDelayMS));
    this->avgPh = this->phSensor->receiveReading();
    vTaskDelay(pdMS_TO_TICKS(100 + this->ecSensor->responseDelayMS - this->phSensor->responseDelayMS));
    this->avgEc = this->ecSensor->receiveReading();
    if (this->phSensor->responseError != 1)
    {
        error = (ErrorCode)(this->phSensor->responseError * 10 + 1);
    }
    else if (this->ecSensor->responseError != 1)
    {
        error = (ErrorCode)(this->ecSensor->responseError * 10 + 2);
    }
    return error;
}

ErrorCode Tank::readPh()
{
    ErrorCode error = success;
    this->avgPh = this->phSensor->readEzoSensor();
    if (this->phSensor->responseError != 1)
    {
        error = (ErrorCode)(this->phSensor->responseError * 10 + 1);
    }
    return error;
}

ErrorCode Tank::readEc()
{
    ErrorCode error = success;
    this->avgEc = this->ecSensor->readEzoSensor();
    if (this->ecSensor->responseError != 1)
    {
        error = (ErrorCode)(this->ecSensor->responseError * 10 + 2);
    }
    return error;
}

ErrorCode Tank::readLevel()
{
    ErrorCode error = success;
    this->waterLevel = this->levelSensor->readLevelSensor();
    return error;
}

ErrorCode Tank::readTemp()
{
    ErrorCode error = success;
    this->waterTemp = this->tempSensor->readTempSensor();
    return error;
}