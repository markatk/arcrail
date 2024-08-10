#include "led.h"

#include "timer.h"

#define LED_MODE_OFF 0
#define LED_MODE_BLINK 1

#ifdef USE_LEDS
typedef struct {
    uint8_t mode;
    uint8_t delay;
    uint8_t time;

    uint8_t pin;
    // TODO: Add invert support
} led_t;

led_t _leds[LED_COUNT];

void initialize_led(led_t *led, uint8_t pin);
void update_led(led_t *led);
void write_led(led_t *led, uint8_t value);
void toggle_led(led_t *led);
uint8_t read_led(led_t *led);
#endif

#ifdef STATUS_LED
uint8_t _status_led_mode;
uint8_t _status_led_delay;
uint8_t _status_led_time;

uint8_t read_status_led();
void write_status_led(uint8_t value);
void toggle_status_led();
#endif

void led_init() {
    for (uint8_t i = 0; i < LED_COUNT; i++) {
        initialize_led(&_leds[i], LEDS[i]);
    }

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

    for (uint8_t i = 0; i < LED_COUNT; i++) {
        update_led(&_leds[i]);
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

#ifdef USE_LEDS
void led_set(uint8_t led, uint8_t value) {
    if (led >= LED_COUNT) {
        return;
    }

    write_led(&_leds[led], value);

    // disable any other mode if it is on
    _leds[led].mode = LED_MODE_OFF;
}

void led_blink(uint8_t led) {
    if (led >= LED_COUNT) {
        return;
    }

    _leds[led].time = 5;
    _leds[led].mode = LED_MODE_BLINK;

    // set initial delay value
    _leds[led].delay = _leds[led].time;

    write_led(&_leds[led], HIGH);
}

void initialize_led(led_t *led, uint8_t pin) {
    pinMode(pin, OUTPUT);

    led->mode = LED_MODE_OFF;
    led->delay = 0;
    led->time = 0;
    led->pin = pin;

    write_led(led, LOW);
}

void update_led(led_t *led) {
    if (led->mode == LED_MODE_BLINK) {
        // wait till the delay is over
        if (led->delay > 0) {
            led->delay -= 1;
        } else {
            toggle_led(led);

            led->delay = led->time;
        }
    }
}

void write_led(led_t *led, uint8_t value) {
    digitalWrite(led->pin, value);
}

void toggle_led(led_t *led) {
    write_led(led, !read_led(led));
}

uint8_t read_led(led_t *led) {
    return digitalRead(led->pin);
}
#endif

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
