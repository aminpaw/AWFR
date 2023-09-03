#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/i2c.h>
#include "ezo_sensor.h"
#include "tank.h"
#include "parameters.h"
#include "handler.h"
#include "lcd_display.hpp"
// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

int main()
{
    stdio_init_all();

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    EzoSensor phSensor = EzoSensor(PhSensor);
    EzoSensor ecSensor = EzoSensor(EcSensor);
    LevelSensor levelSensor = LevelSensor(8, 9);
    TempSensor tempSensor = TempSensor(27);
    Pump floodingPump = Pump(10);
    Pump drainingPump = Pump(11);
    Pump acidPump = Pump(12);
    Pump fertilizerPump = Pump(13);
    Valve mixingValve = Valve(14);
    Valve inputValve = Valve(15);
    Valve outputValve = Valve(16);

    Tank tank = Tank(&phSensor, &ecSensor, &levelSensor, &tempSensor, &floodingPump, &drainingPump, &acidPump, &fertilizerPump, &mixingValve, &inputValve, &outputValve);
    Controller controller = Controller(&tank);
    LCDdisplay display(18, 19, 20, 21, 16, 17, 16, 2); // DB4, DB5, DB6, DB7, RS, E, character_width, no_of_lines
    Handler mainHandler = Handler(&tank, &controller, &display);

    stdio_init_all();

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    printf("STARTING SYSTEM\n PLEASE TYPE c TO CONTINUE\n");
    while (getchar_timeout_us(100) != PICO_ERROR_TIMEOUT)
    {
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        sleep_ms(500);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        sleep_ms(500);
    }
    stdio_flush();
    char flush = getchar();
    printf("FLUSHING BUFFER\n");
    getchar_timeout_us(100);
    sleep_ms(100);

    i2c_init(i2c_default, 200 * 1000);

    gpio_set_function(21, GPIO_FUNC_I2C);
    gpio_set_function(20, GPIO_FUNC_I2C);

    while (1)
    {
        mainHandler.initSystem();
    }
}
