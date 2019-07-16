#include "arduino.h"
#include "toggle.h"

//pins the toggle switch is plugged into.
Toggle::Toggle(int _pin_1, int _pin_2) {
    pin_1 = _pin_1;
    pin_2 = _pin_2;

    pinMode(pin_1, INPUT_PULLUP);
    pinMode(pin_2, INPUT_PULLUP);
}

// 0 means neither is active, 1 means pin 1 is active, 2 means pin 2 is active
int Toggle::getState() {
    int val_1 = digitalRead(pin_1);
    if (val_1 == 0) {
        return 1;
    }

    int val_2 = digitalRead(pin_2);
    if (val_2 == 0) {
        return 2;
    }

    return 0;
}
