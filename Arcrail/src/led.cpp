#include "led.h"

#include "timer.h"

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

void led_init() {
    for (uint8_t i = 0; i < LED_COUNT; i++) {
        initialize_led(&_leds[i], LEDS[i]);
    }
}

void led_update() {
    // wait till timer was triggered
    if (timer_was_triggered() == false) {
        return;
    }

    for (uint8_t i = 0; i < LED_COUNT; i++) {
        update_led(&_leds[i]);
    }
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

bool led_get(uint8_t led) {
    if (led >= LED_COUNT) {
        return false;
    }

    return read_led(&_leds[led]);
}

void led_toggle(uint8_t led) {
    if (led >= LED_COUNT) {
        return;
    }

    toggle_led(&_leds[led]);
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

void led_flash(uint8_t led) {
    if (led >= LED_COUNT) {
        return;
    }

    _leds[led].time = 1;
    _leds[led].mode = LED_MODE_FLASH;

    // set initial delay value
    _leds[led].delay = _leds[led].time;

    write_led(&_leds[led], HIGH);
}

uint8_t led_get_mode(uint8_t led) {
    if (led >= LED_COUNT) {
        return LED_MODE_OFF;
    }

    return _leds[led].mode;
}

    #ifdef STATUS_LED
void status_led_set(uint8_t value) {
    led_set(STATUS_LED, value);
}

bool status_led_get() {
    return led_get(STATUS_LED);
}

void status_led_blink() {
    led_blink(STATUS_LED);
}

void status_led_flash() {
    led_flash(STATUS_LED);
}

uint8_t status_led_get_mode() {
    return led_get_mode(STATUS_LED);
}
    #endif

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
    } else if (led->mode == LED_MODE_FLASH) {
        // wait till the delay is over
        if (led->delay > 0) {
            led->delay -= 1;
        } else {
            write_led(led, LOW);

            led->mode = LED_MODE_OFF;
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
