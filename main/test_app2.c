/*
 * test_app2.c
 * This file is part of litepix
 *
 * Copyright (C) 2015 - Florian Rommel
 *
 * litepix is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * litepix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with litepix. If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdbool.h>
#include <stdlib.h>
#include "main.h"
#include "core/timer.h"
#include "core/pix.h"
#include "util/transitions_step.h"


struct context {
    t_tr_state state;
    uint8_t canvas[PIX_NUM_BYTES];
    uint8_t order[PIX_NUM_PIXELS];
    int8_t mode;
    void (*handler)(struct context*);
};


void fade_handler(struct context* c);
void dissolve_handler(struct context* c);


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


/*
 * Initializes or switches mode
 * Sets state, canvas and handler according to mode
 * Initial mode value must be -1
 */
void switch_mode(struct context* c) {
    c->state.step_count = 250;
    c->state.step_curr = 0; // TODO use get_tr_state
    c->state.d_sum = 0;
    
    c->mode++;
    
    switch (c->mode) {
        case 0:
            fill(c->canvas, 0, 255, 0);
            c->handler = fade_handler;
            break;
        case 1:
            fill(c->canvas, 0, 0, 255);
            break;
        case 2:
            fill(c->canvas, 255, 0, 255);
            break;
        case 3:
            c->handler = dissolve_handler;
            fill(c->canvas, 0, 0, 0);
            tr_shuffle(c->order, PIX_NUM_PIXELS);
            break;
        case 4:
            fill_random(c->canvas);
            tr_shuffle(c->order, PIX_NUM_PIXELS);
            break;
        case 5:
            fill(c->canvas, 255, 0, 0);
        default:
            // Reset mode and handler
            c->mode = -1;
            c->handler = fade_handler;
    }
}


/*
 * Handles fade event
 */
void fade_handler(struct context* c) {
    bool ret = tr_fade_step_p(&c->state, c->canvas);
    if (ret) pix_render();
    else switch_mode(c);
}

/*
 * Handles dissolve event
 */
void dissolve_handler(struct context* c) {
    bool ret = tr_dissolve_step_p(&c->state, c->canvas, c->order);
    if (ret) pix_render();
    else switch_mode(c);
}


/**
 * App main Function
 */
void test_app2(void) {
    // Initialize random generator
    // TODO -> no fixed number
    srand(46721);

    // Create application context
    struct context context;
    
    // Initialize order array with numbers from
    // 1 to PIX_NUM_PIXELS -> will be shuffled later
    uint8_t i;
    for (i = 0; i < PIX_NUM_PIXELS; i++) {
        context.order[i] = i;
    }
    
    // Initialize context members
    context.mode = -1;
    switch_mode(&context);
    
    t_timer timer = timer_get(0);
    
    // On timer event call active handler
    // (either fade_handler or dissolve_handler)
    while (1) {
        if (timer_test(&timer, 16)) context.handler(&context);
    }
}
