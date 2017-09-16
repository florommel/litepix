/*
 * ui.cpp
 * This file is part of Litepix
 *
 * Copyright (C) 2015 - Florian Rommel
 *
 * Litepix is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Litepix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Litepix. If not, see <http://www.gnu.org/licenses/>.
 */


#include <gtk/gtk.h>
#include <cairo.h>
#include <string.h>
#include <stdlib.h>
#include <queue>
#include "../canvas.hpp"
#include "../input.hpp"


static const int Offset = 10;
static const int PixelSize = 30;
static const int RectWidth = PixelSize*Canvas::Width+2*Offset;
static const int RectHeight = PixelSize*Canvas::Height+2*Offset;

static uint8_t save_buffer[Canvas::Bytes] = {0};
static GMutex save_buffer_mutex;
static GtkWidget *window;

static Delegate<void(Input)> input_handler;
static std::queue<Input> input_queue;

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr,
                              gpointer data) {
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_rectangle(cr, 0, 0, RectWidth, RectHeight);
    cairo_fill(cr);

    g_mutex_lock(&save_buffer_mutex);
    uint8_t *p = save_buffer;
    int i, j;
    for (i = 0; i < Canvas::Height; ++i) {
        for (j = 0; j < Canvas::Width; ++j) {
            uint8_t g = *p++;
            uint8_t r = *p++;
            uint8_t b = *p++;
            cairo_set_source_rgb(cr, ((double)r)/256,
                                     ((double)g)/256,
                                     ((double)b)/256);
#ifndef REVERSE_HORIZONTAL
            cairo_rectangle(cr, j*PixelSize+Offset, i*PixelSize+Offset, 28, 28);
#else
            cairo_rectangle(cr, j*PixelSize+Offset, (Canvas::Height-i-1)*PixelSize+Offset, 28, 28);
#endif
            cairo_fill(cr);
        }
    }
    g_mutex_unlock(&save_buffer_mutex);
    return FALSE;
}


static void on_window_destroy() {
    exit(EXIT_SUCCESS);
}


static gboolean on_key_press(GtkWidget *widget, GdkEventKey *event,
                             gpointer user_data) {
    //printf("key: %X\n", event->keyval);
    Input::Data d;
    switch (event->keyval) {
        case 0xFF51:
            d = Input::Left;
            break;
        case 0xFF52:
            d = Input::Up;
            break;
        case 0xFF53:
            d = Input::Right;
            break;
        case 0xFF54:
            d = Input::Down;
            break;
        case 0xFF0D:
            d = Input::Ok;
            break;
        case 0xFF1B:
            d = Input::Exit;
    }

    Input inp(1, d);
    input_queue.push(inp);
    return TRUE;
}


static gpointer gtk_thread_func(gpointer data) {
    gtk_main();
    return NULL;
}


static void init_pix() __attribute__((constructor));
static void init_pix() {
    GtkWidget *darea;

    gtk_init(NULL, NULL);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    darea = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), darea);

    g_signal_connect(G_OBJECT(darea), "draw",
                     G_CALLBACK(on_draw_event), NULL);
    g_signal_connect(window, "destroy",
                     G_CALLBACK(on_window_destroy), NULL);
    g_signal_connect (G_OBJECT(window), "key_press_event",
                      G_CALLBACK(on_key_press), NULL);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), RectWidth, RectHeight);
    gtk_window_set_title(GTK_WINDOW(window), "Simulator");
    gtk_widget_show_all(window);

    g_thread_new("pix_thread", gtk_thread_func, NULL);
}


void Canvas::render() {
    g_mutex_lock(&save_buffer_mutex);
    memcpy(save_buffer, buffer, Bytes);
    g_mutex_unlock(&save_buffer_mutex);
    gtk_widget_queue_draw(window);
}


void Input::set_handler(Delegate<void(Input)> handler) {
    input_handler = handler;
}


void Input::clear_handler() {
    input_handler.clear();
}


void Input::clear_events() {
    input_queue = std::queue<Input>();
}


void Input::handle() {
    if (!input_queue.empty()) {
        if (!input_handler.empty()) input_handler(input_queue.front());
        input_queue.pop();
    }
}
