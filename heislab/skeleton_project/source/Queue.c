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

struct Targs {
    struct Queue* q;
    enum Floor floor;
    enum Direction dirn;
    bool* shouldStop;
};

void _QueueChackIfShoudStopInFloor(void* args) {
    struct Targs* targs = (struct Targs*)args;
    struct Queue* q = targs->q;
    enum Floor floor = targs->floor;
    enum Direction dirn = targs->dirn;
    bool* shouldStop = targs->shouldStop;

    while (q->watchingQueueForStop) {
        struct QueueElement* current = q->head;
        for (int i = 0; i < q->size; i++) {
            if (current == NULL) break;
            if (current->floor == floor) {
                if (current->type ==  Q_OFF || (current->type == Q_UP && dirn == UP) || (current->type == Q_DOWN && dirn == DOWN)) {
                    *shouldStop = true;
                    break;
                }
            }
            current = current->prev;
        }
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }
    return;
}

void QueueCheckIfShouldStopInFloorWatch(struct Queue* q, enum Floor floor, enum Direction dirn, bool* shouldStop) {
    pthread_t watchQueueThread;
    struct Targs* targs = (struct Targs*)malloc(sizeof(struct Targs));
    targs->q = q;
    targs->floor = floor;
    targs->dirn = dirn;
    targs->shouldStop = shouldStop;

    if (q->watchingQueueForStop) {
        printf("{Queue.c}Queue is already being watched for stop");
        return;
    }
    q->watchingQueueForStop = true;
    pthread_create(&watchQueueThread, NULL, _QueueChackIfShoudStopInFloor, targs);
    pthread_detach(watchQueueThread);
}

void QueueStopWatching(struct Queue* q) {
    q->watchingQueueForStop = false;
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
    queue->watchingQueueForStop = false;
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

void QueueRemove(struct Queue* q, struct QueueElement* qe) {
    struct QueueElement* current = q->head;
    struct QueueElement* prev = NULL;
    for (int i = 0; i < q->size; i++) {
        if ((current->floor == qe->floor) && (current->type == qe->type)) {
            printf("removing element\n");
            if (prev == NULL) {
                q->head = current->prev;
            } else {
                prev->prev = current->prev;
            }
            q->size--;
            return;
        }
        prev = current;
        current = current->prev;
    }
}

void QueueDestruct(struct Queue* q) {
    QueueClear(q);
    free(q);
}
