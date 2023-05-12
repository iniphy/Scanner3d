#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdint.h>

extern uint8_t motor_speed;
extern volatile uint16_t enk1_cnt;
extern volatile uint16_t enk2_cnt;
volatile struct flags {
    char int0;
};

#endif