#pragma once
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "./Queue.h"
#include "./Elevator.h"

struct Stopbutton {
    bool stop;
    struct Queue* queue;
    struct Elevator* elevator;
};

struct Stopbutton* StopbuttonSingleton(struct Queue* queue, struct Elevator* elevator);

void* StopbuttonUpdate(void* arg);

void _StopbuttonSetStop(struct Stopbutton* stopbutton, bool stop);