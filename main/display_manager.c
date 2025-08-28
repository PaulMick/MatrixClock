#include <stdio.h>

#include "display_driver.h"
#include "display_utils.h"

DisplayHandle displayHandle;

void display_init() {
    printf("Initializing Display\n");

    displayHandle = get_display_handle();
    int load_font_result = load_font();

    run_refresh();
}

void display_update() {

}