#pragma once
#include "driver/elevio.h"
#include <stdbool.h>
#include <stdlib.h>
#include "./Floor.h"
#include <stdio.h>
#include <pthread.h>

enum Direction {
    UP = 1,
    DOWN = 0
};

struct Elevator {
    enum Floor floor;
    enum Floor lastValidFloor; // Last floor the elevator was at, that is: this value is never 0.
    enum Direction direction;
    bool active;
    bool waiting;
};

void _ElevatorResetMotors(struct Elevator* elevator);

void ElevatorSetActive(struct Elevator* elevator, bool active_);

void ElevatorSetDirection(struct Elevator* elevator, enum Direction direction_);

void _ElevatorSetFloor(struct Elevator* elevator, enum Floor floor_);

void _ElevatorReadFloor(struct Elevator* elevator);

void ElevatorUpdate(struct Elevator* elevator);

void ElevatorReadFloorThread(struct Elevator* elevator);

struct Elevator* ElevatorSingleton();

void ElevatorDestroy(struct Elevator* elevator);

void ElevatorPrint(struct Elevator* elevator);

void ElevatorWaitForNextFloor(struct Elevator* elevator, enum Floor prevFloor);