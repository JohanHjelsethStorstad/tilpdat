#pragma once
#include "driver/elevio.h"
#include <stdbool.h>
#include <stdlib.h>
#include "./Floor.h"

enum Direction {
    UP = 1,
    DOWN = 0
};

struct Elevator {
    enum Floor floor;
    enum Direction direction;
    bool active;
};

void _ElevatorResetMotors(struct Elevator* elevator);

void ElevatorSetActive(struct Elevator* elevator, bool active_);

void ElevatorSetDirection(struct Elevator* elevator, enum Direction direction_);

void _ElevatorSetFloor(struct Elevator* elevator, enum Floor floor_);

void ElevatorReadFloor(struct Elevator* elevator);

struct Elevator* ElevatorSingleton();

void ElevatorDestroy(struct Elevator* elevator);