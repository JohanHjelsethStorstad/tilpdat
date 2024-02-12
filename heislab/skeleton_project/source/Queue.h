#pragma once
#include "./Floor.h"
#include <stdbool.h>
#include <stdlib.h>


enum QueueType = {
    UP,
    DOWN,
    OFF
}

struct QueueElement {
    enum QueueType type;
    enum Floor floor;
}

struct Queue {
    struct QueueElement* elements;
    int length;
}

struct Queue* QueueSingleton();

void QueueClear(struct Queue* queue) {
    free(queue.elements);
    queue.length = 0;
    return
}

void QueueAdd(struct Queue* queue, struct QueueElement* qe) {
    queue.length++;
    queue.elements = realloc(queue.elements, queue.length*sizeof(struct QueueElement));
    queue.elements[queue.length - 1] = qe;
    return;
}