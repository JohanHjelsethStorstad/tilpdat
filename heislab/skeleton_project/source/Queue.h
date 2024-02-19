#pragma once
#include "./Floor.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

enum QueueType {
    Q_UP = 0,
    Q_DOWN = 1,
    Q_OFF = 2
};

struct QueueElement {
    enum QueueType type;
    enum Floor floor;
    struct QueueElement* prev;
};

struct Queue {
    struct QueueElement* head;
    struct QueueElement* tail;
    int size;
    int limit;
};

struct QueueElement* QueueElementConsturctor(enum QueueType type_, enum Floor floor_);

struct Queue* QueueSingleton(int limit);

bool QueueIsEmpty(struct Queue* q);

void QueueDestruct(struct Queue* q);

struct QueueElement* QueuePop(struct Queue *q);

int QueueAdd(struct Queue* q, struct QueueElement* qe);

void QueuePrint(struct Queue* q);

void QueueClear(struct Queue* q);