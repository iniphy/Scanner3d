#include <Arduino.h>
#include "PCF8574.h"
#include "defines.h"
#include "global_vars.h"
#include "interrupts.h"
#include "control.h"

volatile struct motor motor;
volatile struct analogs analogs;
PCF8574 expander(0x39);

void setup() {
    
    Serial.begin(9600);
    Serial.println("Communication init baudrate 9600");

    init_GPIO();
    expander.begin(0);
    
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
        delay(20);
        expander.write8(analogs.val_to_leds);
    }

    for(int i = 100; i >= 20; i--) {
        set_motor_speed(i, RIGHT);
        delay(20);
        expander.write8(analogs.val_to_leds);
    }

    for(int i = 20; i <= 100; i++) {
        set_motor_speed(i, LEFT);
        delay(20);
        expander.write8(analogs.val_to_leds);
    }

    for(int i = 100; i >= 20; i--) {
        set_motor_speed(i, LEFT);
        delay(20);
        expander.write8(analogs.val_to_leds);
    }
    

}

