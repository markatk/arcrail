#include "buttons.h"

#ifdef USE_BUTTONS
typedef struct {
    bool pressed : 1;
    bool just_pressed : 1;
    bool released : 1;
    bool just_released : 1;
    bool invert : 1;

    uint32_t counter;
    uint8_t pin;
} button_t;

button_t _buttons[BUTTON_COUNT];

void _initialize_button(button_t *button, uint8_t pin);
void _update_button(button_t *button);
#endif

void buttons_init() {
    for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
        _initialize_button(&_buttons[i], BUTTONS[i]);
    }
}

void buttons_update() {
    for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
        _update_button(&_buttons[i]);
    }
}

#ifdef USE_BUTTONS
bool button_is_just_pressed(uint8_t button) {
    if (button >= BUTTON_COUNT) {
        return false;
    }

    return _buttons[button].just_pressed;
}

bool button_is_pressed(uint8_t button) {
    if (button >= BUTTON_COUNT) {
        return false;
    }

    return _buttons[button].pressed;
}

bool button_is_just_released(uint8_t button) {
    if (button >= BUTTON_COUNT) {
        return false;
    }

    return _buttons[button].just_released;
}

bool button_is_released(uint8_t button) {
    if (button >= BUTTON_COUNT) {
        return false;
    }

    return _buttons[button].released;
}

void _initialize_button(button_t *button, uint8_t pin) {
    // setup pin
    pinMode(pin, INPUT_PULLUP);

    button->pin = pin;
    button->pressed = false;
    button->released = true;
    button->counter = 0;

    // TODO: Support invert per button
    #ifdef BUTTONS_INVERT
    button->invert = true;
    #else
    button->invert = false;
    #endif
}

void _update_button(button_t *button) {
    bool pressed = digitalRead(button->pin);

    if (button->invert) {
        pressed = !pressed;
    }

    if (button->just_pressed) {
        button->just_pressed = false;
    }

    if (button->just_released) {
        button->just_released = false;
    }

    if (pressed) {
        // button is pressed and marked as pressed
        if (button->pressed) {
            return;
        }

        // button is pressed and waiting to be marked pressed
        button->counter++;

        if (button->counter >= BUTTON_PRESS_COUNT) {
            button->pressed = true;
            button->just_pressed = true;
            button->released = false;
            button->counter = 0;
        }
    } else {
        // button is released and already marked as released in last frame thus clear release flag
        if (button->released) {
            return;
        }

        // button is released and waiting to be marked released
        button->counter++;

        if (button->counter >= BUTTON_RELEASE_COUNT) {
            button->released = true;
            button->just_released = true;
            button->pressed = false;
            button->counter = 0;
        }
    }
}
#endif
