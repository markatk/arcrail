#include "led.h"

#include "timer.h"

#define LED_MODE_OFF 0
#define LED_MODE_BLINK 1

#ifdef STATUS_LED
uint8_t _status_led_mode;
uint8_t _status_led_delay;
uint8_t _status_led_time;

uint8_t read_status_led();
void write_status_led(uint8_t value);
void toggle_status_led();
#endif

void led_init() {
#ifdef STATUS_LED
    pinMode(PIN_STATUS_LED, OUTPUT);

    status_led_set(LOW);
#endif
}

void led_update() {
    // wait till timer was triggered
    if (timer_was_triggered() == false) {
        return;
    }

#ifdef STATUS_LED
    if (_status_led_mode == LED_MODE_BLINK) {
        // wait till the delay is over
        if (_status_led_delay > 0) {
            _status_led_delay -= 1;
        } else {
            toggle_status_led();

            _status_led_delay = _status_led_time;
        }
    }
#endif
}

#ifdef STATUS_LED
void status_led_set(uint8_t value) {
    write_status_led(value);

    _status_led_mode = LED_MODE_OFF; // do not use any special mode
}

void status_led_blink() {
    _status_led_time = 5;
    _status_led_mode = LED_MODE_BLINK;

    // set initial delay value
    _status_led_delay = _status_led_time;

    write_status_led(true);
}

uint8_t read_status_led() {
    return digitalRead(PIN_STATUS_LED);
}

void write_status_led(uint8_t value) {
    #ifdef STATUS_LED_INVERT_OUTPUT
    digitalWrite(PIN_STATUS_LED, !value);
    #else
    digitalWrite(PIN_STATUS_LED, value);
    #endif
}

void toggle_status_led() {
    write_status_led(!read_status_led());
}
#endif
