#include "temp_sensor.h"

float TempSensor::readTempSensor()
{
    this->wire->single_device_read_rom(address);
    this->wire->convert_temperature(address, true, false);
    this->temparature = this->wire->temperature(address);
    return this->temparature;
}