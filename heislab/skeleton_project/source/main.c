#include <stdio.h>
#include <stdlib.h>
#include "driver/elevio.h"
#include "./Controller.h"

int main() {
    elevio_init();
    
    printf("=== Heis max 100 000 000 ===\n");
    
    struct Controller* controller = ControllerSingleton();
    ControllerStartPosition(controller);
    ControllerStart(controller);

    return 0;
}
