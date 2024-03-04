#pragma once
#include "driver/elevio.h"
#include <stdbool.h>
#include <stdlib.h>
#include "./Queue.h"
#include "./Button.h"
#include "./Elevator.h"
#include "./Door.h"
#include "./Stopbutton.h"
#include <signal.h>
#include <time.h>
#include <stdio.h>


struct Controller {
    struct Elevator* elevator;
    struct Door* door;
    struct Queue* queue;
    struct QueueElement* target;
    struct Button* buttons[10];
    struct Stopbutton* stopbutton;
    bool active; //ignore orders and stop button
};

struct Controller* ControllerSingleton();

void ControllerStart(struct Controller* controller);

void ControllerStartPosition(struct Controller* controller);

void ControllerNewTarget(struct Controller* controller);

void QueueClearInForCurrentFloor(struct Controller* controller);

void ControllerStopInThisFloor(struct Controller* controller);

void ControllerActivate(struct Controller* controller);

void ControllerUnactivate(struct Controller* controller);