#include "./Queue.h"

struct Queue* QueueSingleton() {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    return queue;
}