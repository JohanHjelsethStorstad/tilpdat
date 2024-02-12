#pragma once
#include "driver/elevio.h"
#include <stdbool.h>
#include <stdlib.h>

struct Controller {
    struct Elevator* elevator;
};

struct Controller* ControllerSingleton() {
    struct Controller* controller = (struct Controller*)malloc(sizeof(struct Controller));
    return controller;
};