#include <Arduino.h>
#include "defines.h"
#include "interrupts.h"
#include "control.h"

void init_motor(enum motor_curr_limit currLimit, enum motor_decay decay) {
    
    if(currLimit == LIMIT29) {digitalWrite(CURR_I4, LOW); digitalWrite(CURR_I3, LOW); digitalWrite(CURR_I2, HIGH);}
    else if(currLimit == LIMIT47) {digitalWrite(CURR_I4, LOW); digitalWrite(CURR_I3, HIGH); digitalWrite(CURR_I2, LOW);}
    else if(currLimit == LIMIT63) {digitalWrite(CURR_I4, LOW); digitalWrite(CURR_I3, HIGH); digitalWrite(CURR_I2, HIGH);}
    else if(currLimit == LIMIT77) {digitalWrite(CURR_I4, HIGH); digitalWrite(CURR_I3, LOW); digitalWrite(CURR_I2, LOW);}
    else if(currLimit == LIMIT88) {digitalWrite(CURR_I4, HIGH); digitalWrite(CURR_I3, LOW); digitalWrite(CURR_I2, HIGH);}
    else if(currLimit == LIMIT96) {digitalWrite(CURR_I4, HIGH); digitalWrite(CURR_I3, HIGH); digitalWrite(CURR_I2, LOW);}
    else {digitalWrite(CURR_I4, HIGH); digitalWrite(CURR_I3, HIGH); digitalWrite(CURR_I2, HIGH);}
    
    if(decay == SLOW)
        digitalWrite(DECAY, LOW);
    else
        digitalWrite(DECAY, HIGH);
    
    motor.turns_x10 = 0;
    motor.percent = 0;
    digitalWrite(nSLEEP, HIGH);
}

void deinit_motor(void) {
    digitalWrite(nSLEEP, LOW);
}

void set_motor_speed(uint8_t percent, enum motor_dir direction) {
    
    motor.percent = map(percent, 0, 100, 0, 255);

    if(direction == RIGHT) {
        if(motor.freq != 0)
            motor.dir = RIGHT;
        digitalWrite(DRV_IN1, LOW);
        analogWrite(DRV_IN2, motor.percent);
    }
    else if(direction == LEFT) {
        if(motor.freq != 0)
            motor.dir = LEFT;
        analogWrite(DRV_IN1, motor.percent);
        digitalWrite(DRV_IN2, LOW);
        
    }
}
