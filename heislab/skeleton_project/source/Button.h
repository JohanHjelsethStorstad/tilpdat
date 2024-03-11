#pragma once
#include "./Floor.h"
#include "./Queue.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "driver/elevio.h"
#include <pthread.h>

#define N_BTNS 10

struct Button {
    enum QueueType type;
    enum Floor floor;
    struct Queue* queue;
    bool active;
    bool* activeForPress;
};

struct Button* ButtonConstructor(enum QueueType type_, enum Floor floor_, struct Queue* q);

void ButtonDestruct(struct Button* btn);

void _ButtonActivate(struct Button* btn);

void ButtonUnactivate(struct Button* btn);

void _ButtonRead(void* arg);

void ButtonReadThread(struct Button* btn);

void ButtonPrint(struct Button* btn);

void ButtonSetActivePress(struct Button* btn, bool* activeForPress);

