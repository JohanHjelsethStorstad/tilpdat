#pragma once
#include <stdbool.h>
#include "./Timer.h"
#include <time.h>

struct Door {
    bool open;
    struct Timer* doorTimer;
};

struct Door* DoorConstructor();

void DoorOpen(struct Door* door);

void DoorWaitForClose(struct Door* door);

void DoorClose(struct Door* door);

void DoorDestruct(struct Door* door);

void* DoorWatchThread(void* arg);

void DoorWatch(struct Door* door);

void DoorUpdate(void* arg);

void DoorPrint(struct Door* door);