#include "tank.h"
typedef enum ControllerStatus
{
    idle_controller,
    fill_running,
    fill_complete,
    regulate_running,
    regulate_complete,
    flood_running,
    flood_complete
} ControllerStatus;

class Controller
{
public:
    Tank *tank;
    ControllerStatus status = idle_controller;

    Controller(Tank *tank)
    {
        this->tank = tank;
    }

    void fillTank();
    void regulatePhEc();
    void regulatePh();
    void floodWater();
};