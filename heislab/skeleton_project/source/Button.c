#include "./Button.h"

struct Button* ButtonConstructor(enum QueueType type_, enum Floor floor_, struct Queue* q) {
    struct Button* btn = (struct Button*) malloc(sizeof(struct Button));
    btn->type = type_;
    btn->floor = floor_;
    btn->queue = q;
    ButtonUnactivate(btn);
    ButtonReadThread(btn);
    return btn;
}

void ButtonDestruct(struct Button* btn) {
    free(btn);
}

void _ButtonActivate(struct Button* btn) {
    if (!btn->active) {
        struct QueueElement* qe = QueueElementConsturctor(btn->type, btn->floor);
        QueueAdd(btn->queue, qe);
        btn->active = 1;
        elevio_buttonLamp(btn->floor - 1, btn->type, 1);
    } else {
        printf("tried to add an button event to queue, but was already added\n");
    }
}

void ButtonUnactivate(struct Button* btn) {
    btn->active = false;
    elevio_buttonLamp(btn->floor - 1, btn->type, 0);
}

void _ButtonRead(void* arg) {
    struct Button* btn = (struct Button*)arg;
    while (true) {
        int btnPressed = elevio_callButton(btn->floor - 1, btn->type);
        if (btnPressed) {
            _ButtonActivate(btn);
        }
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }
}

void ButtonReadThread(struct Button* btn) {
    pthread_t watchButtonThread;
    pthread_create(&watchButtonThread, NULL, _ButtonRead, btn);
    pthread_detach(watchButtonThread);
}

void ButtonPrint(struct Button* btn) {
    printf("Button: type: %d, floor: %d, active: %d\n", btn->type, btn->floor, btn->active);
}
