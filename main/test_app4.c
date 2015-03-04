#include <stdbool.h>
#include <stdlib.h>
#include "main.h"
#include "core/timer.h"
#include "core/pix.h"
#include "util/transitions_step.h"


static void fill(uint8_t* canvas, uint8_t r, uint8_t g, uint8_t b) {
    uint8_t i;
    uint8_t* p = canvas;
    for (i = 0; i < PIX_NUM_PIXELS; i++) {
        *p++ = g;
        *p++ = r;
        *p++ = b;
    }
}

static void fill_random(uint8_t* canvas) {
    uint8_t i;
    uint8_t* p = canvas;
    for (i = 0; i < PIX_NUM_PIXELS; i++) {
        *p++ = rand() % 256;
        *p++ = rand() % 256;
        *p++ = rand() % 256;
    }
}

void set_state1(t_tr_state* state, uint8_t* canvas) {
    *state = get_tr_state(250);
    fill(canvas, 0, 0, 255);
}


void set_state2(t_tr_state* state, uint8_t* canvas) {
    *state = get_tr_state(60);
    if (rand() & 0x01) fill(canvas, 255, 0, 0);
    else fill_random(canvas);
}


/**
 * App main Function
 */
void test_app4(void) {
    srand(46721);

    uint8_t canvas[PIX_NUM_BYTES];
    t_tr_state state;
    
    set_state1(&state, canvas);
    
    bool mode = true;
    t_timer timer = timer_get(0);
    
    // On timer event call active handler
    // (either fade_handler or dissolve_handler)
    while (1) {
        if (timer_test(&timer, 16)) {
            bool ret = tr_roll_step_p(&state, canvas);
            if (ret) {
                pix_render();
            } else {
                if (mode) set_state2(&state, canvas);
                else set_state1(&state, canvas);
                mode = !mode;
                timer = timer_get(200);
            }
        }
    }
}
