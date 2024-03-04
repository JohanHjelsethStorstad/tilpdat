#include "./Controller.h"

struct Controller* ControllerSingleton() {
    struct Controller* controller = (struct Controller*)malloc(sizeof(struct Controller));
    controller->elevator = ElevatorSingleton();
    controller->door = DoorSingleton();
    controller->queue = QueueSingleton(2000);
    controller->target = NULL;
    
    int i = 0;
    for(int f = 1; f <= 4 ; f++){
        for(int t = 0; t <= 2; t++){
            if (!(f == 1 && t == 1) && !(f == 4 && t== 0)) {
                printf("making button %d\n", i);
                (controller->buttons)[i] = ButtonConstructor(t, f, controller->queue); 
                i++;
            }
        }
    }
    controller->stopbutton = StopbuttonSingleton(controller);

    return controller;
}

void ControllerStart(struct Controller* controller) {
    ElevatorSetActive(controller->elevator, true);
    ElevatorSetDirection(controller->elevator, UP);
    while (1) {
        ElevatorPrint(controller->elevator);
        QueuePrint(controller->queue);

        if (controller->target) {
            int dirn = UP;
            if (controller->target->floor == controller->elevator->lastValidFloor) {
                dirn = controller->elevator->direction == UP ? DOWN : UP; 
            } else {
                dirn = controller->target->floor > controller->elevator->lastValidFloor ? UP : DOWN;
            }
            ElevatorSetDirection(controller->elevator, dirn);
            if (controller->elevator->floor == controller->target->floor) {
                controller->target = NULL;
                DoorOpen(controller->door);
                DoorWatch(controller->door);
                DoorWaitForClose(controller->door);
                queueClearInForCurrentFloor(controller);
            } else {
                ElevatorSetActive(controller->elevator, true);
                ElevatorWaitForNextFloor(controller->elevator, controller->elevator->floor);
                int diff = -1;
                if (controller->target != NULL) {
                    diff = controller->target->floor - controller->elevator->floor;
                }
                if (diff == 0) {
                    ElevatorSetActive(controller->elevator, false);    
                    queueClearInForCurrentFloor(controller);
                    controller->target = NULL;
                    DoorOpen(controller->door);
                    DoorWatch(controller->door);
                    DoorWaitForClose(controller->door);
                }
            }
        } else {
            ElevatorSetActive(controller->elevator, false);
            ControllerNewTarget(controller);
        }
    }
}

void queueClearInForCurrentFloor(struct Controller* controller) {
    for (int i=0; i < 10; ++i) {
        struct Button* currentBtn = (controller->buttons)[i];
        if (currentBtn->floor == controller->elevator->floor) {
            ButtonUnactivate(currentBtn);
        }
    }
}

void ControllerStartPosition(struct Controller* controller){

}

void ControllerNewTarget(struct Controller* controller) {
    struct QueueElement* new = QueuePop(controller->queue);
    if (new == NULL) {
        //printf("No more queue events yet\n");
    } else {
        controller->target = new;
    }
}

