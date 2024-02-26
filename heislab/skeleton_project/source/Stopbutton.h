#pragma once
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "./Controller.h"

struct Stopbutton {
    bool stop;
    struct Controller* controller;
};

struct Stopbutton* StopbuttonSingleton(struct Controller* controller);

void* StopbuttonUpdate(void* arg);

void _StopbuttonSetStop(struct Stopbutton* stopbutton, bool stop);