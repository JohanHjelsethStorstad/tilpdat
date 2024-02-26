#pragma once
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

struct Timer {
    int time;
    int stopTime;
    bool running;
    bool halted;
};

struct Timer* TimerConstructor(int duration);

void TimerDestruct(struct Timer* timer);

void TimerReset(struct Timer* timer);

void* TimerUpdate(void* arg);

void TimerStart(struct Timer* timer);

void TimerOff();

void TimerOn(struct Timer* timer);

void TimerHalt(struct Timer* timer);

void TimerUnhalt(struct Timer* timer);
