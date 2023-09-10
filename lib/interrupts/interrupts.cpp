#include <Arduino.h>
#include "defines.h"
#include "global_vars.h"
#include "interrupts.h"

uint8_t timer1_prescaler;
uint8_t timer2_prescaler;
uint8_t val_to_compare;

ISR(INT0_vect) {
    motor.enk1_cnt++;
}

ISR(INT1_vect) {
    motor.enk2_cnt++;
}



ISR(TIMER1_COMPA_vect) {
    if(timer1_prescaler < 59) //Freq 1Hz
        timer1_prescaler++;
    else {
        timer1_prescaler = 0;
        // TODO: for the moment this communication (monitoring) is not handled in the PhysioServer
        //     : thus, we need to turned it off as it disrupt the normal communication otherwise 
        // Serial.print("Dir:  ");
        // if(motor.dir == RIGHT)
        //     Serial.print("RIGHT");
        // else if(motor.dir == LEFT)
        //     Serial.print("LEFT");
        // else
        //     Serial.print("UNDEF");
        // Serial.print("\tFreq:  ");
        // Serial.print(motor.freq);
        // Serial.print("\tTurns:  ");
        // Serial.print(motor.turns_x10 / 10);

        analogs.batt_volt = (uint16_t)(analogRead(BATT_V) * 9) / 205; // 9 * 5V / 1024bit, VERY slow decay due to huge RC
        analogs.temp = (uint16_t)(analogRead(TEMP) * 135) / 205 - 172; // temp(u) = 135.415 * 5V / 1024bit * u - 171.872

        // Serial.print("\tBatt:  ");
        // Serial.print(analogs.batt_volt);
        // Serial.print("\tTemp:  ");
        // Serial.println(analogs.temp);

        val_to_compare = analogs.temp;
        if(val_to_compare <= TRSH1)
            analogs.val_to_leds = 0b00000001;
        else if(val_to_compare > TRSH1 && val_to_compare <= TRSH2)
            analogs.val_to_leds = 0b00000011;
        else if(val_to_compare > TRSH2 && val_to_compare <= TRSH3)
            analogs.val_to_leds = 0b00000111;
        else if(val_to_compare > TRSH3 && val_to_compare <= TRSH4)
            analogs.val_to_leds = 0b00001111;
        else if(val_to_compare > TRSH4)
            analogs.val_to_leds = 0b00011111;
        else
            analogs.val_to_leds = 0b00000000;

    }
}

ISR(TIMER2_COMPA_vect) {
    if(timer2_prescaler < 9) //Freq 10Hz
        timer2_prescaler++;
    else {
        timer2_prescaler = 0;
        
        uint16_t turns_x10 = (max(motor.enk1_cnt, motor.enk2_cnt) * 10) / 14;
        
        if(motor.dir == RIGHT)
            motor.turns_x10 += turns_x10;
        else if(motor.dir == LEFT)
            motor.turns_x10 -= turns_x10;
        
        motor.freq = turns_x10; // 14 pulses for revolution, and 10 measurements per second

        motor.enk1_cnt = 0;
        motor.enk2_cnt = 0;

        if(motor.freq == 0)
            motor.dir = UNDEF;
        
        digitalWrite(WDO, !digitalRead(WDO)); // Reset Watchdog timer
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



void timer1_init(void) {
    
    TCCR1B = TIMSK1 = 0; // Reset default Arduino configuration
    OCR1A = 255;//5624; // Value for freq
    TCCR1B |= (1 << WGM12); // CTC mode
    TCCR1B |= (1 << CS12); // Prescaler
    TIMSK1 |= (1 << OCIE1A); // Enable Compare match

    timer1_prescaler = 0;
}

void timer2_init(void) {

    TCCR2A = TCCR2B = TIMSK2 = 0; // Reset default Arduino configuration
    OCR2A = 38; // Value for freq = 100Hz
    TCCR2A |= (1 << WGM01); // CTC mode
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20); // Prescaler
    TIMSK2 |= (1 << OCIE2A); // Enable Compare match

    timer2_prescaler = 0;
}
