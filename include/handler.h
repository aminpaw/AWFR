#ifndef HANDLER_H
#define HANDLER_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "parameters.h"
#include <string>
#include "tank.h"
#include "controller.h"
#include "util.h"
#include "lcd_display.hpp"

typedef enum Status
{
    waiting_user_input,
    ready,
    controller_running,
    controller_complete,
    reading_sensors,
    idle
} Status;

typedef enum Mode
{
    not_selected,
    ph_regulate,
    ph_ec_regulate,
    ec_regulate,
    flood,
    monitor
} Mode;

class Handler
{
public:
    Tank *tank;
    LCDdisplay *display;
    Controller *controller;
    Status status = waiting_user_input;
    Mode mode = not_selected;
    bool printingFlag = false;

    void initSystem();

    static void monitoring(void *);
    Mode selectMode();
    static void phRegulateMode(void *);

    static void readPh(void *pvParams)
    {
        while (1)
        {
            ((Tank *)pvParams)->readPh();
            vTaskDelay(pdMS_TO_TICKS(250));
        }
    }

    static void readLevel(void *pvParams)
    {
        while (1)
        {
            ((Tank *)pvParams)->readLevel();
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }

    static void readTemp(void *pvParams)
    {
        while (1)
        {
            ((Tank *)pvParams)->readTemp();
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }

    Handler(Tank *tank, Controller *controller, LCDdisplay *display)
    {
        this->tank = tank;
        this->controller = controller;
        this->display = display;
    }

    static void blink(void *pvParams)
    {
        while (1)
        {
            gpio_put(PICO_DEFAULT_LED_PIN, 1);
            vTaskDelay(pdMS_TO_TICKS(1000));
            gpio_put(PICO_DEFAULT_LED_PIN, 0);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
};

#endif