#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "parameters.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

char readSerialFirstChar();
float readSerialFloat();
int readSerialInt();
string floatToString(float value, int precision);

#endif