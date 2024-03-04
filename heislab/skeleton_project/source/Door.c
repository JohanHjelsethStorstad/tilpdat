#include "./Door.h"

struct Door* DoorSingleton() {
    struct Door* door = (struct Door*)malloc(sizeof(struct Door));
    door->open = false;
    door->doorTimer = TimerConstructor(3);
    return door;
}

void DoorOpen(struct Door* door) {
    elevio_doorOpenLamp(1);
    door->open = true;
    TimerReset(door->doorTimer);
    TimerStart(door->doorTimer); 
    return;
}

void DoorWaitForClose(struct Door* door) {
    while (door->open) {
        DoorWatch(door);
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }
    return;
}

void DoorClose(struct Door* door) {
    elevio_doorOpenLamp(0);
    door->open = false;
    return;
}

void DoorDestruct(struct Door* door) {
    TimerDestruct(door->doorTimer);
    free(door);
    return;
}

void* DoorWatchThread(void* arg) {
    struct Door* door = (struct Door*)arg;
    while (door->open) {
        DoorUpdate(door);
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }
    return NULL;
}

void DoorWatch(struct Door* door) {
    pthread_t watchDoorThread;
    pthread_create(&watchDoorThread, NULL, DoorWatchThread, door);
    pthread_detach(watchDoorThread);
}

void DoorUpdate(void* arg) {
    struct Door* door = (struct Door*)arg;
    //TODO check obstruction -> reset timer
    if (elevio_obstruction()) {
        TimerReset(door->doorTimer);
        TimerHalt(door->doorTimer);
    } else {
        TimerUnhalt(door->doorTimer);
    }

    if (door->open && !(door->doorTimer->running)) {
        DoorClose(door);
    }
    return;
}

void DoorPrint(struct Door* door) {
    printf("Door: %s\n", door->open ? "Open" : "Closed");
    printf("DoorTimer: %d\n", door->doorTimer->time);
    return;
}