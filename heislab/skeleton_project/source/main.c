#include <stdio.h>
#include <stdlib.h>
#include "driver/elevio.h"
#include "./Controller.h"

int main() {
    elevio_init();
    
    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");


    
    struct Controller* controller = ControllerSingleton();
    ControllerStart(controller);
    

    return 0;
}
