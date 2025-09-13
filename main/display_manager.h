#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include "modes_assets.h"

void display_init(int loading_screen);

void display_update(modestates_t mode_states, current_mode_t mode);

#endif