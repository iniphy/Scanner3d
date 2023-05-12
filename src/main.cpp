#include <Arduino.h>
#include "defines.h"
#include "globals.h"
#include "init.h"

ISR(INT0_vect) {
    enk1_cnt++;
}

ISR(INT1_vect) {
    enk2_cnt++;
}

uint8_t motor_speed = 0;
volatile uint16_t enk1_cnt;
volatile uint16_t enk2_cnt;

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
    // pinMode(WDO, OUTPUT);

    pinMode(BATT_V ,INPUT);
    pinMode(TEMP ,INPUT);

    Serial.begin(9600);

    digitalWrite(nSLEEP, HIGH);
    digitalWrite(DECAY, LOW);
    digitalWrite(CURR_I2, LOW);
    digitalWrite(CURR_I3, LOW);
    digitalWrite(CURR_I4, HIGH);

    interrupts_init();
    sei();
}

void loop() {

    analogWrite(DRV_IN1, motor_speed);
    motor_speed++;
    digitalWrite(DRV_IN2, HIGH);
    delay(100);
    //Serial.println(analogRead(BATT_V));
    //Serial.println(analogRead(TEMP));
}