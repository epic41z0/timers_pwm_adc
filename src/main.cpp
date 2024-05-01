#include "timer.h"
#include <avr/io.h>

int main() {
    setup();

    while (1) {
        // uppdaterar LED-läge i main loopen
        startUpLed();
    }

    return 0;
}
