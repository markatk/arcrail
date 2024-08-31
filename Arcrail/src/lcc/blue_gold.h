#pragma once

#include "../../configuration.h"
#include "types.h"

#ifdef LCC_USE_BLUE_GOLD
    #define LCC_BLUE_GOLD_MODE_OFF 0
    #define LCC_BLUE_GOLD_MODE_LEARN 1
    #define LCC_BLUE_GOLD_MODE_TEACH 2
#endif

void blue_gold_init();

void blue_gold_update();

void blue_gold_reset();

#ifdef LCC_USE_BLUE_GOLD
void blue_gold_learn(lcc_event_id_t event_id);

uint8_t blue_gold_get_mode();

uint8_t blue_gold_get_selection();
#endif
