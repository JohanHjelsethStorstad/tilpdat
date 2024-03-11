#pragma once
#include "./Floor.h"
#include "./Elevator.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

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
    bool watchingQueueForStop;
    int size;
    int limit;
};

struct QueueElement* QueueElementConsturctor(enum QueueType type_, enum Floor floor_);
void QueueElementDestruct(struct QueueElement* qe);

struct Queue* QueueSingleton(int limit);

bool QueueIsEmpty(struct Queue* q);

void QueueDestruct(struct Queue* q);

struct QueueElement* QueuePop(struct Queue *q);

int QueueAdd(struct Queue* q, struct QueueElement* qe);

void QueuePrint(struct Queue* q);

void QueueClear(struct Queue* q);

void _QueueChackIfShoudStopInFloor(void* args);

void QueueCheckIfShouldStopInFloorWatch(struct Queue* q, enum Floor floor, enum Direction dirn, bool* shouldStop);

void QueueStopWatching(struct Queue* q);

void QueueRemove(struct Queue* q, struct QueueElement* qe);