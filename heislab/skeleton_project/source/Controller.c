#include "./Controller.h"

struct Controller* ControllerSingleton() {
    struct Controller* controller = (struct Controller*)malloc(sizeof(struct Controller));
    controller->elevator = ElevatorSingleton();
    controller->door = DoorSingleton();
    controller->queue = QueueSingleton(2000);
    controller->target = NULL;
    controller->active = false;
    
    int i = 0;
    for(int f = 1; f <= 4 ; f++){
        for(int t = 0; t <= 2; t++){
            if (!(f == 1 && t == 1) && !(f == 4 && t== 0)) {
                printf("making button %d\n", i);
                (controller->buttons)[i] = ButtonConstructor(t, f, controller->queue); 
                ButtonSetActivePress((controller->buttons)[i], &controller->active);
                i++;
            }
        }
    }
    controller->stopbutton = NULL;

    return controller;
}

void ControllerStart(struct Controller* controller) {
    controller->active = true;
    controller->stopbutton = StopbuttonSingleton(controller);

    ElevatorSetActive(controller->elevator, true);
    ElevatorSetDirection(controller->elevator, UP);
    while (1) {
        //ElevatorPrint(controller->elevator);
        QueuePrint(controller->queue);
        if (controller->target) {
            int dirn = UP;
            if (controller->target->floor == controller->elevator->lastValidFloor) {
                dirn = controller->elevator->direction == UP ? DOWN : UP; 
            } else {
                dirn = controller->target->floor > controller->elevator->lastValidFloor ? UP : DOWN;
            }
            ElevatorSetDirection(controller->elevator, dirn);
            if (controller->target != NULL && controller->elevator->floor == controller->target->floor) {
                controller->target = NULL;
                DoorOpen(controller->door);
                DoorWatch(controller->door);
                DoorWaitForClose(controller->door);
                QueueClearInForCurrentFloor(controller);
            } else {
                ElevatorSetActive(controller->elevator, true);
                int diff = -1;
                bool shouldStop = false;
                bool* ptrShouldStop = &shouldStop;
                do {
                    if (shouldStop) {
                        ControllerStopInThisFloor(controller);
                        ElevatorSetActive(controller->elevator, true);
                        shouldStop = false;
                    }
                    int nextFloor = controller->elevator->floor + (controller->elevator->direction == UP ? 1 : -1);
                    QueueCheckIfShouldStopInFloorWatch(controller->queue, nextFloor, controller->elevator->direction, ptrShouldStop);
                    ElevatorWaitForNextFloor(controller->elevator, controller->elevator->floor);
                    QueueStopWatching(controller->queue);
                    if (controller->target) {
                        diff = controller->target->floor - controller->elevator->floor;
                    }
                } while (diff != 0 && !controller->stopbutton->stop);

                if (diff == 0) {
                    controller->target = NULL;
                    ControllerStopInThisFloor(controller);
                    ElevatorSetActive(controller->elevator, false);
                } 
            }
        } else {
            ElevatorSetActive(controller->elevator, false);
            ControllerNewTarget(controller);
            nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
        }
    }
}

void ControllerActivate(struct Controller* controller) {
    controller->active = true;
    for (int i=0; i < 10; ++i) {
        ButtonSetActivePress((controller->buttons)[i], &controller->active);
    }
    return;
}

void ControllerStopInThisFloor(struct Controller* controller) {
    ElevatorSetActive(controller->elevator, false);    
    QueueClearInForCurrentFloor(controller);
    DoorOpen(controller->door);
    DoorWatch(controller->door);
    DoorWaitForClose(controller->door);
}

void QueueClearInForCurrentFloor(struct Controller* controller) {
    // clear buttons in floor
    for (int i=0; i < 10; ++i) {
        struct Button* currentBtn = (controller->buttons)[i];
        if (currentBtn->floor == controller->elevator->floor) {
            ButtonUnactivate(currentBtn);
        }
    }

    //clear queue in floor
    //Array of queue elements to remove
    struct QueueElement* toRemove[10];
    int j = 0;

    struct QueueElement* current = controller->queue->head;
    for (int i = 0; i < controller->queue->size; i++) {
        if (current->floor == controller->elevator->floor) {
            printf("adding queue element for remove\n");
            toRemove[j] = current;
            j++;
        }
        current = current->prev;
    }
    for (int i = 0; i < j; i++) {
        QueueRemove(controller->queue, toRemove[i]);
    }
}

void ControllerStartPosition(struct Controller* controller){
    _ElevatorReadFloor(controller->elevator);
    if (controller->elevator->floor == 0) {
        ElevatorSetDirection(controller->elevator, DOWN);
        ElevatorSetActive(controller->elevator, true);
        ElevatorWaitForNextFloor(controller->elevator, 0);
    }
        
}

void ControllerNewTarget(struct Controller* controller) {
    struct QueueElement* new = QueuePop(controller->queue);
    if (new == NULL) {
        //printf("No more queue events yet\n");
    } else {
        controller->target = new;
    }
}

void ControllerDestroy(struct Controller* controller) {
    StopbuttonDestroy(controller->stopbutton);
    ElevatorDestroy(controller->elevator);
    DoorDestruct(controller->door);
    QueueDestruct(controller->queue);
    QueueElementDestruct(controller->target);
    for (int i=0; i<10; ++i) {
        ButtonDestruct(controller->buttons[i]);
    }
}

