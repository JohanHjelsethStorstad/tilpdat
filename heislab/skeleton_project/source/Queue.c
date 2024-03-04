#include "./Queue.h"

struct QueueElement* QueueElementConsturctor(enum QueueType type_, enum Floor floor_) {
    struct QueueElement* qe = (struct QueueElement*) malloc(sizeof(struct QueueElement));
    qe->type = type_;
    qe->floor = floor_;
    qe->prev = NULL;

    return qe;
}

void QueueElementDestruct(struct QueueElement* qe) {
    free(qe);
}

struct Queue* QueueSingleton(int limit) {
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    if (limit <= 0) {
        limit = 2000;
    }
    queue->limit = limit;
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

bool QueueIsEmpty(struct Queue* q) {
    return q->size == 0;
}

struct QueueElement* QueuePop(struct Queue *q) {
    struct QueueElement* element;
    if (QueueIsEmpty(q)) return NULL;
    element = q->head;
    q->head = (q->head)->prev;
    q->size--;
    return element;
}

int QueueAdd(struct Queue* q, struct QueueElement* qe) {
    if (q->size >= q->limit) {
        return 0;
    }
    qe->prev = NULL;
    if (q->size == 0) {
        q->head = qe;
        q->tail = qe;
    } else {
        q->tail->prev = qe;
        q->tail = qe;
    }
    q->size++;
    return 1;
}

void QueueClear(struct Queue* q) {
    while (!QueueIsEmpty(q)) {
        struct QueueElement* qe = QueuePop(q);
        QueueElementDestruct(qe);
    }
} 

void QueuePrint(struct Queue* q) {
    struct QueueElement* current = q->head;
    printf("Queue: \n");
    for (int i = 0; i < q->size; i++) {
        if (current == NULL) break;
        printf("Floor: %d, type: %d\n", current->floor, current->type);
        current = current->prev;
    } 
}

void QueueDestruct(struct Queue* q) {
    QueueClear(q);
    free(q);
}
