#include "./Stopbutton.h"

struct Stopbutton* StopbuttonSingleton(struct Controller* controller) {
    static struct Stopbutton* stopbutton = NULL;
    if (stopbutton == NULL) {
        stopbutton = (struct Stopbutton*)malloc(sizeof(struct Stopbutton));
        stopbutton->controller = controller;

        pthread_t watchStopbuttonThread;
        pthread_create(&watchStopbuttonThread, NULL, StopbuttonUpdate, stopbutton);
        pthread_detach(watchStopbuttonThread);
    }else {
        printf("Stopbutton already initialized\n");
    }
    elevio_stopLamp(0);
    
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
        printf("Stop");
        ElevatorSetActive(stopbutton->controller->elevator, false);
        stopbutton->controller->target = NULL;
        
        QueueClear(stopbutton->controller->queue);
        for (int i=0; i < 10; ++i) {
            struct Button* currentBtn = (stopbutton->controller->buttons)[i];
            ButtonUnactivate(currentBtn);
        }
        if (stopbutton->controller->elevator->floor != 0) {
            DoorOpen(stopbutton->controller->door);
            DoorWatch(stopbutton->controller->door);
        }
    }
}