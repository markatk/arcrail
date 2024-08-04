#include "timer.h"

#if defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328PB__)
    #define TIMER_MAX 50 // timer0 is configured at 2ms, trigger should happen every 100ms
#else
    #error Timer module is not defined for this board!
#endif

uint8_t _trigger_count;
uint8_t _timer_count;

void timer_init() {
    _trigger_count = 0;
    _timer_count = 0;

#if defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328PB__)
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function below
    OCR0A = 0x32;
    TIMSK0 |= _BV(OCIE0A);
#else
    #error Timer module cannot be configured for this board!
#endif
}

bool timer_was_triggered() {
    if (_trigger_count == 0) {
        return false;
    }

    _trigger_count--;

    return true;
}

SIGNAL(TIMER0_COMPA_vect) {
    // only increase count if not already maxed out
    if (_trigger_count == 255) {
        return;
    }

    _timer_count++;
    if (_timer_count < TIMER_MAX) {
        return;
    }

    _trigger_count++;
    _timer_count = 0;
}
