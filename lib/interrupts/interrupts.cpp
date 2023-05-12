#include <Arduino.h>
#include "defines.h"
#include "global_vars.h"
#include "interrupts.h"

ISR(INT0_vect) {
    motor.enk1_cnt++;
}

ISR(INT1_vect) {
    motor.enk2_cnt++;
}

ISR(TIMER0_COMPA_vect) {
    
    if(motor.enk1_cnt == motor.enk2_cnt)
        motor.dir = UNDEF;
    else if(motor.enk1_cnt > motor.enk2_cnt)
        motor.dir = RIGHT;
    else
        motor.dir = LEFT;
    
    uint16_t turns = (max(motor.enk1_cnt, motor.enk2_cnt) / 14);
    motor.turns += turns; 
    motor.freq = turns * 10; // 14 pulses for revolution, and 10 measurements per second

    motor.enk1_cnt = 0;
    motor.enk2_cnt = 0;
}

ISR(TIMER1_COMPA_vect) {
    Serial.print("Dir: ");
    if(motor.dir == RIGHT)
        Serial.print("RIGHT");
    else if(motor.dir == LEFT)
        Serial.print("LEFT");
    else
        Serial.print("UNDEF");

    Serial.print(" Freq: ");
    Serial.print(motor.freq);

    analogs.batt_volt = (uint16_t)(analogRead(BATT_V) * 9) / 205; // 9 * 5V / 1024bit, VERY slow decay due to huge RC
    analogs.temp = (uint16_t)(analogRead(TEMP) * 135) / 205 - 172; // temp(u) = 135.415 * 5V / 1024bit * u - 171.872

    Serial.print(" Batt: ");
    Serial.print(analogs.batt_volt);
    Serial.print(" Temp: ");
    Serial.println(analogs.temp);
}

void int0_init(void) {

    EICRA |= (1 << ISC00);
    EICRA |= (1 << ISC01);  // Trigger INT0 on rising edge
    EIMSK |= (1 << INT0);   // Enable INT0 interrupt

    motor.enk1_cnt = 0;
}

void int1_init(void) {

    EICRA |= (1 << ISC10);
    EICRA |= (1 << ISC11);  // Trigger INT1 on rising edge
    EIMSK |= (1 << INT1);   // Enable INT1 interrupt

    motor.enk2_cnt = 0;
}

void timer0_init(void) {

    TCCR0A = TCCR0B = TIMSK0 = 0;   // Reset default Arduino configuration
    OCR0A = 155;                    // Set the TOP value to achieve a 100ms interrupt interval (f_cpu/256/1000*OCR0A)
    TCCR0A |= (1 << WGM01);         // Set the Timer0 mode to CTC (Clear Timer on Compare)
    TCCR0B |= (1 << CS02);          // Set the Timer0 prescaler to 256 (f_cpu/256)
    TIMSK0 |= (1 << OCIE0A);        // Enable the Timer0 interrupt on Compare A match
}

void timer1_init(void) {
    
    TCCR1B = TIMSK1 = 0;    // Reset default Arduino configuration
    OCR1A = 15624;          // F_CPU / (TIMER_PRESCALER * 1) - 1 Set compare match value to generate 1 second interrupt
    TCCR1B |= (1 << WGM12); // Set the Timer0 mode to CTC (Clear Timer on Compare)
    TCCR1B |= (1 << CS12) | (1 << CS10); // Set timer prescaler value to 1024
    TIMSK1 |= (1 << OCIE1A);// Enable the Timer1 interrupt on Compare A match
}
