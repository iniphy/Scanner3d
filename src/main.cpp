#include <Arduino.h>
#include "defines.h"
#include "global_vars.h"
#include "interrupts.h"

volatile struct motor motor;

void setup() {
    
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

    Serial.begin(9600);
    Serial.println("Communication init baudrate 9600");

    digitalWrite(nSLEEP, HIGH);
    digitalWrite(DECAY, LOW);
    digitalWrite(CURR_I2, LOW);
    digitalWrite(CURR_I3, LOW);
    digitalWrite(CURR_I4, HIGH);
    digitalWrite(WDO, LOW);
    
    cli();
    int0_init();
    int1_init();
    timer0_init();
    timer1_init();
    sei();
}

void loop() {

    digitalWrite(DRV_IN1, HIGH);
    motor.speed++;
    digitalWrite(DRV_IN2, LOW);
    //delay(1000);
    Serial.print(motor.enk1_cnt);
    Serial.println(motor.enk2_cnt);
    // Serial.println(analogRead(BATT_V));
    // Serial.println(analogRead(TEMP));
}