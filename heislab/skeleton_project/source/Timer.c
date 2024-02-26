#include "./Timer.h"

struct Timer* TimerConstructor(int duration) {
    struct Timer* timer = (struct Timer*)malloc(sizeof(struct Timer));
    timer->stopTime = duration;
    timer->running = false;
    timer->halted = false;
    TimerReset(timer);

    pthread_t watchTimeThread;
    pthread_create(&watchTimeThread, NULL, TimerUpdate, timer);
    pthread_detach(watchTimeThread);
    return timer;
}

void TimerDestruct(struct Timer* timer) {
    free(timer);
}

void TimerReset(struct Timer* timer) {
    timer->time = 0;
}

void TimerOff(struct Timer* timer) {
    timer->running = false;
}

void TimerOn(struct Timer* timer) {
    timer->running = true;
}

void* TimerUpdate(void* arg) {
    struct Timer* timer = (struct Timer*)arg;
    while (true) {
        while (!timer->halted && timer->running) {
            sleep(1);
            printf("%d\n", timer->time);
            timer->time++;
            if (timer->time >= timer->stopTime) {
                TimerOff(timer);
            }
        }
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }
    return NULL;
}

void TimerHalt(struct Timer* timer) {
    timer->halted = true;
}

void TimerUnhalt(struct Timer* timer) {
    timer->halted = false;
}

void TimerStart(struct Timer* timer) {
    timer->time = 0;
    timer->running = true;
}