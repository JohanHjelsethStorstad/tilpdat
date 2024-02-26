#pragma once
#include "./Floor.h"
#include "./Queue.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "driver/elevio.h"
#include <pthread.h>

struct Button {
    enum QueueType type;
    enum Floor floor;
    struct Queue* queue;
    bool active;
};

struct Button* ButtonConstructor(enum QueueType type_, enum Floor floor_, struct Queue* q);

void ButtonDestruct(struct Button* btn);

void _ButtonActivate(struct Button* btn);

void ButtonUnactivate(struct Button* btn);

void ButtonRead(struct Button* btn);

void ButtonReadThread(struct Button* btn);

void ButtonPrint(struct Button* btn);

