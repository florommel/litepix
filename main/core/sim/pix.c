/*
 * pix.c
 * This file is part of litepix
 *
 * Copyright (C) 2015 - Florian Rommel, Michael Nieß
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


#include <gtk/gtk.h>
#include <cairo.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../pix.h"
#include "../init.h"


uint8_t pix_canvas[PIX_NUM_BYTES] = {0};

static uint8_t save_canvas[PIX_NUM_BYTES] = {0};
static GMutex save_canvas_mutex;

static GtkWidget *window;
const int size = 100;
const int offset = 10;

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data) {
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_rectangle(cr, 0, 0, (size*PIX_HEIGHT)+2*offset, (size*PIX_WIDTH)+2*offset);
    cairo_fill(cr);
    
    g_mutex_lock(&save_canvas_mutex);
    uint8_t *p = save_canvas;
    int i, j;
    for (i = 0; i < PIX_WIDTH; i++) {
        for (j = 0; j < PIX_HEIGHT; j++) {
            uint8_t g = *p++;
            uint8_t r = *p++;
            uint8_t b = *p++;
            cairo_set_source_rgb(cr, ((double)r)/256,
                                     ((double)g)/256,
                                     ((double)b)/256);
            cairo_rectangle(cr, j*size+offset, i*size+offset, size-2, size-2);
            cairo_fill(cr);
        }
    }
    g_mutex_unlock(&save_canvas_mutex);
    return FALSE;
}


static void on_window_destroy(void) {
    exit(EXIT_SUCCESS);
}


static gpointer gtk_thread_func(gpointer data) {
    gtk_main();
    return NULL;
}


void init_pix(void) {
    GtkWidget *darea;
    
    gtk_init(NULL, NULL);
    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    
    darea = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), darea);
    
    g_signal_connect(G_OBJECT(darea), "draw", 
      G_CALLBACK(on_draw_event), NULL);  
    g_signal_connect(window, "destroy",
      G_CALLBACK(on_window_destroy), NULL);
    
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), (size*PIX_HEIGHT)+2*offset, (size*PIX_WIDTH)+2*offset); 
    gtk_window_set_title(GTK_WINDOW(window), "Simulator");
    
    gtk_widget_show_all(window);

    g_thread_new("pix_thread", gtk_thread_func, NULL);
}


void pix_render(void) {
    g_mutex_lock(&save_canvas_mutex);
    memcpy(save_canvas, pix_canvas, PIX_NUM_BYTES);
    g_mutex_unlock(&save_canvas_mutex);
    gtk_widget_queue_draw(window);
}

