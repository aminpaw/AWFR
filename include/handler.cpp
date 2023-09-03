#include "handler.h"

Mode Handler::selectMode()
{
    this->display->clear();
    this->display->print_wrapped("MODE: 1:PH 2:EC 3:PH&EC 4:READ");
    printf("MODE: 1:PH 2:EC 3:PH&EC 4:READ\n");
    Mode mode = not_selected;
    while (mode == not_selected)
    {
        int modeInput = readSerialInt();
        printf("%i\n", modeInput);
        if (modeInput > 5 || modeInput == 0)
        {
            this->display->clear();
            this->display->print_wrapped("INVALID MODE    SELECT AGAIN!\n");
            printf("INVALID MODE    SELECT AGAIN!\n");
        }
        else
        {
            mode = (Mode)modeInput;
        }
    }
    this->display->clear();
    this->display->print_wrapped("MODE SELECTED");
    printf("MODE SELECTED\n");

    this->mode = mode;
    return mode;
}

void Handler::phRegulateMode(void *pvParams)
{
    Handler *handler = (Handler *)pvParams;
    Controller *controller = handler->controller;
    LCDdisplay *display = handler->display;
    int step = 0;
    while (step != 3)
    {
        if (step == 0)
        {
            display->clear();
            display->print_wrapped("PLEASE INSERT   DESIRED PH:");
            printf("PLEASE INSERT   DESIRED PH:\n");
            float input = readSerialFloat();
            if (input == -1)
            {
                step = 0;
            }
            else
            {
                controller->tank->phSetPoint = input;
                step++;
            }
        }
        if (step == 1)
        {
            display->clear();
            display->print_wrapped("INSERT FLOOD    TIME (MINS):");
            printf("INSERT FLOOD    TIME (MINS):\n");
            float input = readSerialFloat();
            if (input == -1)
            {
                step--;
            }
            else
            {
                controller->tank->floodTimeMin = input;
                step++;
            }
        }
        if (step == 2)
        {
            display->clear();
            display->print_wrapped("INSERT DRAIN    TIME (MINS):");
            printf("INSERT DRAIN    TIME (MINS):\n");
            float input = readSerialFloat();
            if (input == -1)
            {
                step--;
            }
            else
            {
                controller->tank->drainTimeMin = input;
                step++;
            }
        }
    }
    handler->printingFlag = true;
    display->clear();
    display->print_wrapped("----RUNNING----");
    printf("----RUNNING----\n");
    vTaskDelay(pdMS_TO_TICKS(1000));
    handler->printingFlag = false;
    bool firstRun = true;
    while (1)
    {
        absolute_time_t start = get_absolute_time();
        vTaskDelay(pdMS_TO_TICKS(3000));
        controller->fillTank();

        // handler->printingFlag = true;
        display->clear();
        display->print_wrapped("--TANK FILLED--");
        printf("--TANK FILLED--\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
        // handler->printingFlag = false;

        vTaskDelay(pdMS_TO_TICKS(1000));
        controller->regulatePh();

        handler->printingFlag = true;
        display->clear();
        display->print_wrapped("--PH REGULATED--");
        printf("--PH REGULATED--\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
        handler->printingFlag = false;

        vTaskDelay(pdMS_TO_TICKS(1000));
        absolute_time_t end = get_absolute_time();
        int64_t timeDiffMS = absolute_time_diff_us(start, end) * 1000;
        if (firstRun)
        {
            firstRun = false;
        }
        else
        {
            int64_t drainTimeRemain = (controller->tank->drainTimeMin * 60 * 1000) - timeDiffMS;
            if (drainTimeRemain > 0)
            {
                vTaskDelay(pdMS_TO_TICKS(drainTimeRemain));
            }
        }
        controller->floodWater();

        handler->printingFlag = true;
        display->clear();
        display->print_wrapped("---FLOOD DONE---");
        printf("---FLOOD DONE---\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
        handler->printingFlag = false;
        vTaskDelay(pdMS_TO_TICKS(1000));

        handler->status = controller_complete;
        vTaskDelay(pdMS_TO_TICKS(1000));

        handler->printingFlag = true;
        display->clear();
        display->print_wrapped("----COMPLETE----");
        printf("----COMPLETE----\n");
    }
}

void Handler::monitoring(void *pvParams)
{
    Tank *tank = ((Handler *)pvParams)->tank;
    bool *printingFlag = &((Handler *)pvParams)->printingFlag;
    LCDdisplay *display = ((Handler *)pvParams)->display;
    vTaskDelay(pdMS_TO_TICKS(1000));
    while (1)
    {
        // string OldDisplayMsg = "PH: " + floatToString(tank->avgPh, 1) + " T:" + floatToString(tank->waterTemp, 1) + "C" + "  Level:" + floatToString(tank->waterLevel, 1);
        if (*printingFlag == false)
        { // printf("PH: %f    Level: %f    Temp: %fC\n", tank->avgPh, tank->waterLevel, tank->waterTemp);
            string displayMsg = "PH: " + floatToString(tank->avgPh, 1) + " T:" + floatToString(tank->waterTemp, 1) + "C" + "  Level:" + floatToString(tank->waterLevel, 1) + "   ";
            printf("%s\n", displayMsg.c_str());
            const char *c = displayMsg.c_str();
            // display->clear();
            display->cursor_off();
            display->print_wrapped(c);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        else
        {
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}

void Handler::initSystem()
{
    TaskHandle_t readPhHandler = NULL;
    TaskHandle_t monitorHandler = NULL;
    TaskHandle_t readLevelHandler = NULL;
    TaskHandle_t regulatePhHandler = NULL;
    TaskHandle_t readTempHandler = NULL;

    this->display->init();
    // uint32_t blinkStatus = xTaskCreate(
    //     Handler::blinkError,
    //     "Status LED",
    //     1024,
    //     (void *)this->tank,
    //     1,
    //     NULL);

    this->selectMode();

    uint32_t readPhStatus = xTaskCreate(
        Handler::readPh,
        "Read Ph",
        1024,
        (void *)this->tank,
        1,
        &readPhHandler);

    uint32_t readLevelStatus = xTaskCreate(
        Handler::readLevel,
        "Read Level",
        1024,
        (void *)this->tank,
        1,
        &readLevelHandler);

    uint32_t readTempStatus = xTaskCreate(
        Handler::readTemp,
        "Read Temp",
        1024,
        (void *)this->tank,
        1,
        &readTempHandler);

    uint32_t blinkStatus = xTaskCreate(
        Handler::blink,
        "Blink",
        1024,
        NULL,
        1,
        NULL);
    if (this->mode == ph_regulate)
    {
        uint32_t regulatePhStatus = xTaskCreate(
            Handler::phRegulateMode,
            "Regulate Ph",
            3072,
            (void *)this,
            1,
            &regulatePhHandler);
    }

    uint32_t monitorStatus = xTaskCreate(
        Handler::monitoring,
        "Monitor Readings",
        1024,
        (void *)this,
        1,
        &monitorHandler);

    vTaskStartScheduler();

    while (this->status != controller_complete)
    {
        vTaskDelay(pdMS_TO_TICKS(1));
    }
    vTaskDelete(regulatePhHandler);
    vTaskDelete(monitorHandler);
    vTaskDelete(readPhHandler);
    vTaskDelete(readLevelHandler);
    vTaskDelete(readTempHandler);

    printf("---COMPLETE!!!---\n");
}