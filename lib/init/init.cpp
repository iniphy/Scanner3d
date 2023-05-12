#include "Arduino.h"
#include "globals.h"
#include "init.h"

void pinInterrupts_init(void) {

    EICRA |= (1 << ISC00);
    EICRA |= (1 << ISC01);  // Trigger INT0 on rising edge
    EIMSK |= (1 << INT0);   // Enable INT0 interrupt

    EICRA |= (1 << ISC10);
    EICRA |= (1 << ISC11);  // Trigger INT1 on rising edge
    EIMSK |= (1 << INT1);   // Enable INT1 interrupt

    motor.enk1_cnt = motor.enk2_cnt = 0;
}

void timers_init(void) {

    TCCR0A |= (1 << WGM01);  // Set the Timer0 mode to CTC (Clear Timer on Compare)
    TCCR0B |= (1 << CS02);   // Set the Timer0 prescaler to 256 (f_cpu/256)
    OCR0A = 155;             // Set the TOP value to achieve a 100ms interrupt interval (f_cpu/256/1000*OCR0A)
    TIMSK0 |= (1 << OCIE0A); // Enable the Timer0 interrupt on Compare A match


    TCCR1B |= (1 << WGM12); // Set the Timer0 mode to CTC (Clear Timer on Compare)
    TCCR1B |= (1 << CS12) | (1 << CS10); // Set timer prescaler value to 1024
    OCR1A = 15624; // F_CPU / (TIMER_PRESCALER * 1) - 1 Set compare match value to generate 1 second interrupt
    TIMSK1 |= (1 << OCIE1A); // Enable the Timer1 interrupt on Compare A match
}