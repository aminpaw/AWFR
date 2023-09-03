#include "level_sensor.h"

float LevelSensor::readLevelSensor()
{
    int timeout = 26100;
    float avgDistance = 0.0;
    int noSamples = 5;
    for (int i = 0; i < noSamples - 1; i++)
    {
        int trigPin = this->trigPin;
        int echoPin = this->echoPin;
        long duration;
        float distance;
        gpio_set_dir(trigPin, GPIO_OUT);
        gpio_set_dir(echoPin, GPIO_IN);
        gpio_put(trigPin, 1);
        sleep_us(10);
        gpio_put(trigPin, 0);

        uint64_t width = 0;

        while (gpio_get(echoPin) == 0)
            tight_loop_contents();
        absolute_time_t startTime = get_absolute_time();
        while (gpio_get(echoPin) == 1)
        {
            width++;
            sleep_us(1);
            if (width > timeout)
                return this->level;
        }
        absolute_time_t endTime = get_absolute_time();

        float timeDiff = absolute_time_diff_us(startTime, endTime);
        distance = timeDiff / 29.0 / 2.0;
        avgDistance += distance / noSamples;
    }
    // distance += 3;
    // distance = MAXLEVEL - distance;
    this->distance = avgDistance;
    // this->level = TANKHEIGHT - avgDistance;
    return this->level;
}