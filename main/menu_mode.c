#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "modes_assets.h"

modestate_menu_t get_menu_state() {
    modestate_menu_t state = {
        .selection = 0,
    };
    return state;
}