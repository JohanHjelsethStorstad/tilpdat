#include "./Stopbutton.h"

struct Stopbutton* StopbuttonSingleton(struct Controller* controller) {
    static struct Stopbutton* stopbutton = NULL;
    if (stopbutton == NULL) {
        stopbutton = (struct Stopbutton*)malloc(sizeof(struct Stopbutton));
        stopbutton->controller = controller;
        stopbutton->stop = false;
        stopbutton->doorHoldOpen = false;

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

    if (stop && stopbutton->controller->active) {
        printf("Stop");
        ElevatorSetActive(stopbutton->controller->elevator, false);
        stopbutton->controller->target = NULL;
        
        QueueClear(stopbutton->controller->queue);
        for (int i=0; i < 10; ++i) {
            struct Button* currentBtn = (stopbutton->controller->buttons)[i];
            ButtonUnactivate(currentBtn);
        }
        if (stopbutton->controller->elevator->floor != 0) {
            stopbutton->doorHoldOpen = true;
        }
        stopbutton->controller->active = false;
    } else if (!stop && !stopbutton->controller->active) {
        ControllerActivate(stopbutton->controller);
        stopbutton->doorHoldOpen = false;
        TimerStart(stopbutton->controller->door->doorTimer);
    }
    if (stopbutton->doorHoldOpen) {
        TimerHalt(stopbutton->controller->door->doorTimer);
        DoorOpen(stopbutton->controller->door);
        DoorWatch(stopbutton->controller->door);
    }
}