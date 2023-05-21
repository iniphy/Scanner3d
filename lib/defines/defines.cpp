#include "defines.h"
#include <Arduino.h>

void init_GPIO(void) {
    
    pinMode(BTN1 ,INPUT);
    pinMode(BTN2 ,INPUT);
    pinMode(EXP_INT ,INPUT);
    pinMode(nFAULT ,INPUT);
    pinMode(ENK1 ,INPUT);
    pinMode(ENK2 ,INPUT);

    pinMode(DRV_IN1, OUTPUT);
    pinMode(DRV_IN2, OUTPUT);
    pinMode(DECAY, OUTPUT);
    pinMode(nSLEEP, OUTPUT);
    pinMode(CURR_I2, OUTPUT);
    pinMode(CURR_I3, OUTPUT);
    pinMode(CURR_I4, OUTPUT);
    pinMode(WDO, OUTPUT);

    pinMode(BATT_V ,INPUT);
    pinMode(TEMP ,INPUT);
    pinMode(SPARE, OUTPUT);

    digitalWrite(WDO, HIGH);
}