#include "ezo_sensor.h"

float EzoSensor::readEzoSensor()
{
    char msg[] = "r";
    int msgLen = sizeof(msg) / sizeof(msg[0]);

    char readBuf[32];
    reg_write(i2c_default, 10, (uint8_t *)msg, msgLen);
    vTaskDelay(pdMS_TO_TICKS(this->responseDelayMS));
    int bytesRead = reg_read(i2c_default, 10, (uint8_t *)readBuf, 32);
    char *substr = readBuf + 1;
    int errorCode = (int)readBuf[0];
    switch (errorCode)
    {
    case 1:
        this->responseErrorStr = "SUCCESS";
        break;
    case 2:
        this->responseErrorStr = "SYNTAX ERROR";
        break;
    case 254:
        this->responseErrorStr = "NO DATA";
        break;
    case 255:
        this->responseErrorStr = "NOT READY";
        break;
    default:
        this->responseErrorStr = "UNKNOWN ERROR";
        break;
    }
    this->responseError = errorCode;
    if (errorCode == 1)
    {
        this->reading = strtof(substr, NULL);
    }
    return this->reading;
}

void EzoSensor::sendReadCommand()
{
    char msg[] = "r";
    int msgLen = sizeof(msg) / sizeof(msg[0]);

    reg_write(i2c_default, 10, (uint8_t *)msg, msgLen);
}

float EzoSensor::receiveReading()
{
    char readBuf[32];
    int bytesRead = reg_read(i2c_default, 10, (uint8_t *)readBuf, 32);
    char *substr = readBuf + 1;
    int errorCode = (int)readBuf[0];
    switch (errorCode)
    {
    case 1:
        this->responseErrorStr = "SUCCESS";
        break;
    case 2:
        this->responseErrorStr = "SYNTAX ERROR";
        break;
    case 254:
        this->responseErrorStr = "NO DATA";
        break;
    case 255:
        this->responseErrorStr = "NOT READY";
        break;
    default:
        this->responseErrorStr = "UNKNOWN ERROR";
        break;
    }
    this->responseError = errorCode;
    if (errorCode == 1)
    {
        this->reading = strtof(substr, NULL);
    }
    return this->reading;
}

int reg_write(i2c_inst_t *i2c,
              const uint addr,
              uint8_t *buf,
              const uint8_t nbytes)
{

    int num_bytes_read = 0;
    uint8_t msg[nbytes];

    // Check to make sure caller is sending 1 or more bytes
    if (nbytes < 1)
    {
        return 0;
    }

    // Append register address to front of data packet
    // msg[0] = reg;
    for (int i = 0; i < nbytes; i++)
    {
        msg[i] = buf[i];
    }

    // Write data to register(s) over I2C
    i2c_write_blocking(i2c, addr, msg, (nbytes), false);

    return num_bytes_read;
}

int reg_read(i2c_inst_t *i2c,
             const uint addr,
             uint8_t *buf,
             const uint8_t nbytes)
{

    int num_bytes_read = 0;

    // Check to make sure caller is asking for 1 or more bytes
    if (nbytes < 1)
    {
        return 0;
    }

    // Read data from register(s) over I2C
    num_bytes_read = i2c_read_blocking(i2c, addr, buf, nbytes, false);

    return num_bytes_read;
}