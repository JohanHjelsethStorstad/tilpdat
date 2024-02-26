#include "./Stopbutton.h"

struct Stopbutton* StopbuttonSingleton(struct Queue* queue, struct Elevator* elevator) {
    static struct Stopbutton* stopbutton = NULL;
    if (stopbutton == NULL) {
        stopbutton = (struct Stopbutton*)malloc(sizeof(struct Stopbutton));
        _StopbuttonSetStop(stopbutton, false);
        stopbutton->queue = queue;
        stopbutton->elevator = elevator;

        pthread_t watchStopbuttonThread;
        pthread_create(&watchStopbuttonThread, NULL, StopbuttonUpdate, stopbutton);
        pthread_detach(watchStopbuttonThread);
    }else {
        printf("Stopbutton already initialized\n");
    }
    
    return stopbutton;
}

void* StopbuttonUpdate(void* arg) {
    struct Stopbutton* stopbutton = (struct Stopbutton*)arg;
    while (1) {
        _StopbuttonSetStop(stopbutton, elevio_stopButton());
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }
    return NULL;
}

void _StopbuttonSetStop(struct Stopbutton* stopbutton, bool stop) {
    stopbutton->stop = stop;
    elevio_stopLamp(stop);

    if(stop) {
        ElevatorSetActive(stopbutton->elevator, false);
        QueueClear(stopbutton->queue);
    }
}