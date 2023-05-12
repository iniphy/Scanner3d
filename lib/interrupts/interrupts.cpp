#include <Arduino.h>
#include "globals.h"
#include "interrupts.h"

ISR(INT0_vect) {
    motor.enk1_cnt++;
}

ISR(INT1_vect) {
    motor.enk2_cnt++;
}

ISR(TIMER0_COMPA_vect) {
    
}

ISR(TIMER1_COMPA_vect) {
    Serial.print(motor.enk1_cnt);
    Serial.print(" ");
    Serial.println(motor.enk2_cnt);
    motor.enk1_cnt = motor.enk2_cnt = 0;
    }