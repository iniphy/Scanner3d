#ifndef INTERRUPTS_H
#define INTERRUPTS_H

ISR(INT0_vect);         // encoder input 1 counting impulses
ISR(INT1_vect);         // encoder input 2 counting impulses
ISR(TIMER0_COMPA_vect); // timer to count speed out of encoder measuments, called every 100ms
ISR(TIMER1_COMPA_vect); // timer witch calls an interrupt each 1s 

#endif