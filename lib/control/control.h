#ifndef CONTROL_H
#define CONTROL_H

#include "global_vars.h"

void init_motor(enum motor_curr_limit currLimit, enum motor_decay decay);
void deinit_motor(void);
void set_motor_speed(uint8_t percent, enum motor_dir direction);
void motor_fault(void);

#endif