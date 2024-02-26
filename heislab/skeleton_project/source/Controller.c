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

    return controller;
}

void ControllerStart(struct Controller* controller) {
    ElevatorSetActive(controller->elevator, true);
    ElevatorSetDirection(controller->elevator, UP);
    while (1) {
        ElevatorReadFloor(controller->elevator);

        ElevatorPrint(controller->elevator);
        QueuePrint(controller->queue);

        if (!controller->target) {
            ElevatorSetActive(controller->elevator, false);
            ControllerNewTarget(controller);
        } else {
            if (controller->elevator->floor == 0) {

            } else {
                int diff = controller->elevator->floor - controller->target->floor;
                if (diff == 0) {
                    ElevatorSetActive(controller->elevator, false);    
                    for (int i=0; i < 10; ++i) {
                        struct Button* currentBtn = (controller->buttons)[i];
                        if (currentBtn->type == controller->target->type && currentBtn->floor == controller->target->floor) {
                            ButtonUnactivate(currentBtn);
                        }
                    }
                    controller->target = NULL;
                    DoorOpen(controller->door);
                    DoorWatch(controller->door);
                    DoorWaitForClose(controller->door);
                } else {
                    ElevatorSetActive(controller->elevator, true);
                    if (diff > 0)ElevatorSetDirection(controller->elevator, DOWN);
                    else ElevatorSetDirection(controller->elevator, UP);
                } 
            }
        }

        if (controller->elevator->floor != 0) {
            // queue checks
        }
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
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
