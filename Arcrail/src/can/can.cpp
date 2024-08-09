#include "can.h"

#ifdef USE_CAN
    #ifdef CAN_USE_MCP2515
        #include "mcp2515.h"
    #endif
#endif

void can_init() {
#ifdef USE_CAN
    #ifdef CAN_USE_MCP2515
    mcp2515_init();
    mcp2515_change_mode(MCP2515_MODE_NORMAL);
    #endif
#endif
}

void can_update() {
}
