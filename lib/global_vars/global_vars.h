#ifndef GLOBAL_VARS_H
#define GLOBALS_VARS_H

#include <stdint.h>
#include "PCF8574.h"

#pragma once
enum motor_curr_limit {
    LIMIT29,
    LIMIT47,
    LIMIT63,
    LIMIT77,
    LIMIT88,
    LIMIT96,
    LIMIT100
};

enum motor_decay {
    SLOW,
    FAST
};

enum motor_dir {
    UNDEF,
    RIGHT,
    LEFT
};

struct motor {
    uint8_t percent;
    uint16_t freq;
    int32_t turns_x10;
    enum motor_dir dir; 
    uint16_t enk1_cnt;
    uint16_t enk2_cnt;
};
extern volatile struct motor motor;

struct analogs {
    uint16_t temp;
    uint16_t batt_volt;
    uint8_t val_to_leds;
};
extern volatile struct analogs analogs;

#endif