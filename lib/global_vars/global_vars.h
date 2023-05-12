#ifndef GLOBAL_VARS_H
#define GLOBALS_VARS_H

#include <stdint.h>

enum dir {
    RIGHT,
    LEFT,
    UNDEF
};
struct motor{
    uint16_t freq;
    uint16_t turns;
    enum dir dir; 
    uint16_t enk1_cnt;
    uint16_t enk2_cnt;
};
extern volatile struct motor motor;

struct analogs{
    uint16_t temp;
    uint16_t batt_volt;
};
extern volatile struct analogs analogs;

#endif