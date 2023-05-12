#include <Arduino.h>
#include "defines.h"
#include "global_vars.h"
#include "interrupts.h"
#include "control.h"

volatile struct motor motor;
volatile struct analogs analogs;

void setup() {
    
    Serial.begin(9600);
    Serial.println("Communication init baudrate 9600");

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

    
    cli();
    int0_init();
    int1_init();
    timer1_init();
    timer2_init();
    sei();
    init_motor(LIMIT77, SLOW);
}

void loop() {
    for(int i = 20; i <= 100; i++) {
        set_motor_speed(i, RIGHT);
        delay(50);
    }
    for(int i = 100; i >= 20; i--) {
        set_motor_speed(i, RIGHT);
        delay(50);
    }
    for(int i = 20; i <= 100; i++) {
        set_motor_speed(i, LEFT);
        delay(50);
    }
    for(int i = 100; i >= 20; i--) {
        set_motor_speed(i, LEFT);
        delay(50);
    }
}