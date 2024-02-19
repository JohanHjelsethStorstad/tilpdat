#include "./Elevator.h"

void _ElevatorResetMotors(struct Elevator* elevator) {
    if (!(elevator->active)) {
        elevio_motorDirection(DIRN_STOP);
        return;
    }
    if (elevator->direction == UP) {
        elevio_motorDirection(DIRN_UP);
        return;
    }
    elevio_motorDirection(DIRN_DOWN);
}

void ElevatorSetActive(struct Elevator* elevator, bool active_) {
    elevator->active = active_;
    _ElevatorResetMotors(elevator);
    return;
}


void ElevatorSetDirection(struct Elevator* elevator, enum Direction direction_) {
    elevator->direction = direction_;
    _ElevatorResetMotors(elevator);
    return;
}

void _ElevatorSetFloor(struct Elevator* elevator, enum Floor floor_) {
    elevator->floor = floor_;
    return;
}

void ElevatorReadFloor(struct Elevator* elevator) {
    int floor = elevio_floorSensor();
    _ElevatorSetFloor(elevator, floor + 1);
}

struct Elevator* ElevatorSingleton() {
    struct Elevator* elevator = (struct Elevator*)malloc(sizeof(struct Elevator));
    ElevatorReadFloor(elevator);
    ElevatorSetActive(elevator, false);
    ElevatorSetDirection(elevator, DOWN);
    return elevator;
}

void ElevatorDestroy(struct Elevator* elevator) {
    free(elevator);
    return;
}

void ElevatorPrint(struct Elevator* elevator) {
    printf("Elevator: ");
    printf("Active: %d, floor: %d, direction: %d \n", elevator->active, elevator->floor, elevator->direction);
}