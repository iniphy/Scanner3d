#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdint.h>

enum dir {
    RIGHT,
    LEFT,
    UNDEF
};
struct motor{
    uint8_t speed;
    uint16_t turns;
    enum dir dir; 
    uint16_t enk1_cnt;
    uint16_t enk2_cnt;
};
extern volatile struct motor motor;

#endif