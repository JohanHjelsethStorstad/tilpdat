#include "./Timer.h"

struct Timer* TimerConstructor(int duration) {
    struct Timer* timer = (struct Timer*)malloc(sizeof(struct Timer));
    timer->startTime = 0;
    timer->stopTime = duration;
    timer->running = false;
    TimerReset(timer);
    return timer;
}

void TimerDestruct(struct Timer* timer) {
    free(timer);
}

void TimerReset(struct Timer* timer) {
    timer->time = timer->startTime;
    timer->running = false;
}

void* TimerUpdate(void* arg) {
    struct Timer* timer = (struct Timer*)arg;
    while (timer->running) {
        sleep(1);
        timer->time++;
        if (timer->time >= timer->stopTime) {
            timer->running = false;
        }
    }
    return NULL;
}

void TimerStart(struct Timer* timer) {
    timer->running = true;

    pthread_t watchTimeThread;
    pthread_create(&watchTimeThread, NULL, TimerUpdate, timer);

    pthread_detach(watchTimeThread);
}