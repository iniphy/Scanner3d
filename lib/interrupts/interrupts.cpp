#include <Arduino.h>
#include "defines.h"
#include "global_vars.h"
#include "interrupts.h"

uint8_t timer0_prescaler;
uint8_t timer1_prescaler;

ISR(INT0_vect) {
    motor.enk1_cnt++;
}

ISR(INT1_vect) {
    motor.enk2_cnt++;
}

ISR(TIMER0_COMPA_vect) {
    if(timer0_prescaler < 9) //Freq 10Hz
        timer0_prescaler++;
    else {
        timer0_prescaler = 0;
        if(motor.enk1_cnt == motor.enk2_cnt)
            motor.dir = UNDEF;
        else if(motor.enk1_cnt > motor.enk2_cnt)
            motor.dir = RIGHT;
        else
            motor.dir = LEFT;
        
        uint16_t turns_x10 = (max(motor.enk1_cnt, motor.enk2_cnt) * 10) / 14;
        motor.turns_x10 += turns_x10; 
        motor.freq = turns_x10; // 14 pulses for revolution, and 10 measurements per second

        motor.enk1_cnt = 0;
        motor.enk2_cnt = 0;
    }
}

ISR(TIMER1_COMPA_vect) {
    if(timer1_prescaler < 59) //Freq 1Hz
        timer1_prescaler++;
    else {
        timer1_prescaler = 0;
        Serial.print("Dir: ");
        if(motor.dir == RIGHT)
            Serial.print("RIGHT");
        else if(motor.dir == LEFT)
            Serial.print("LEFT");
        else
            Serial.print("UNDEF");

        Serial.print(" Freq: ");
        Serial.print(motor.freq);
        Serial.print(" Turns: ");
        Serial.print(motor.turns_x10 / 10);

        analogs.batt_volt = (uint16_t)(analogRead(BATT_V) * 9) / 205; // 9 * 5V / 1024bit, VERY slow decay due to huge RC
        analogs.temp = (uint16_t)(analogRead(TEMP) * 135) / 205 - 172; // temp(u) = 135.415 * 5V / 1024bit * u - 171.872

        Serial.print(" Batt: ");
        Serial.print(analogs.batt_volt);
        Serial.print(" Temp: ");
        Serial.println(analogs.temp);
    }
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

    TCCR0A = TCCR0B = TIMSK0 = 0; // Reset default Arduino configuration
    OCR0A = 38; // Value for freq = 100Hz
    TCCR0A |= (1 << WGM01); // CTC mode
    TCCR0B |= (1 << CS02) | (1 << CS00); // Prescaler
    TIMSK0 |= (1 << OCIE0A); // Enable Compare match

    timer0_prescaler = 0;
}

void timer1_init(void) {
    
    TCCR1B = TIMSK1 = 0; // Reset default Arduino configuration
    OCR1A = 255;//5624; // Value for freq
    TCCR1B |= (1 << WGM12); // CTC mode
    TCCR1B |= (1 << CS12); // Prescaler
    TIMSK1 |= (1 << OCIE1A); // Enable Compare match

    timer1_prescaler = 0;
}
