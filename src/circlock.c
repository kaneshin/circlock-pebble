// circlock.c
//
// Copyright (c) 2014 Shintaro Kaneko (http://kaneshinth.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "circlock.h"

#include <pebble.h>

#include "circlock_conf.h"
#include "circlock_bg.h"
#include "circlock_hands.h"

Window *window;

Layer *bg_layer;
Layer *date_layer;
Layer *hands_layer;
// Layer *battery_layer;

TextLayer *day_label;
char day_buffer[6];
TextLayer *num_label;
char num_buffer[4];

static void date_update_proc(Layer *layer, GContext *ctx)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    strftime(day_buffer, sizeof(day_buffer), "%a", t);
    text_layer_set_text(day_label, day_buffer);

    strftime(num_buffer, sizeof(num_buffer), "%d", t);
    text_layer_set_text(num_label, num_buffer);
}

static void handle_second_tick(struct tm *tick_time, TimeUnits units_changed)
{
    layer_mark_dirty(window_get_root_layer(window));
}

static void battery_update_proc(Layer *layer, GContext *ctx)
{
//     graphics_context_set_stroke_color(ctx, CIRCLOCK_COLOR_FOREGROUND);
//     graphics_draw_line(ctx, GPoint(19, 54), GPoint(123, 54));
}

static void handle_battery(BatteryChargeState charge_state)
{
//     uint8_t charged = charge_state.charge_percent;
    
//     Layer *window_layer = window_get_root_layer(window);
//     GRect bounds = layer_get_bounds(window_layer);
      
//     layer_set_update_proc(battery_layer, battery_update_proc);
}

static void window_load(Window *window)
{
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    GPoint center = grect_center_point(&bounds);

    // init layers
    bg_layer = layer_create(bounds);
    layer_set_update_proc(bg_layer, circlock_bg_update_proc);
    layer_add_child(window_layer, bg_layer);
    
    // init date layer -> a plain parent layer to create a date update proc
    date_layer = layer_create(bounds);
    layer_set_update_proc(date_layer, date_update_proc);
    layer_add_child(window_layer, date_layer);

    // init day
    day_label = text_layer_create(GRect(46, center.y - 10, 27, 20));
    text_layer_set_text(day_label, day_buffer);
    text_layer_set_background_color(day_label, GColorClear);
    text_layer_set_text_color(day_label, CIRCLOCK_COLOR_FOREGROUND);
    GFont norm18 = fonts_get_system_font(FONT_KEY_GOTHIC_18);
    text_layer_set_font(day_label, norm18);

    layer_add_child(date_layer, text_layer_get_layer(day_label));

    // init num
    num_label = text_layer_create(GRect(73, center.y - 10, 18, 20));

    text_layer_set_text(num_label, num_buffer);
    text_layer_set_background_color(num_label, GColorClear);
    text_layer_set_text_color(num_label, CIRCLOCK_COLOR_FOREGROUND);
    GFont bold18 = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
    text_layer_set_font(num_label, bold18);

    layer_add_child(date_layer, text_layer_get_layer(num_label));

    // init hands
    hands_layer = layer_create(bounds);
    layer_set_update_proc(hands_layer, circlock_hands_update_proc);
    layer_add_child(window_layer, hands_layer);
}

static void window_unload(Window *window)
{
//     layer_destroy(battery_layer);
    layer_destroy(hands_layer);
    text_layer_destroy(num_label);
    text_layer_destroy(day_label);
    layer_destroy(date_layer);
    layer_destroy(bg_layer);
}

void circlock_init()
{
    window = window_create();
    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
    
    day_buffer[0] = '\0';
    num_buffer[0] = '\0';
    
    circlock_bg_init();
    circlock_hands_init(window_get_root_layer(window));
    
    const bool animated = true;
    window_stack_push(window, animated);

    tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);
    
//     battery_state_service_subscribe(&handle_battery);
}

void circlock_deinit()
{
    circlock_hands_deinit();
    circlock_bg_deinit();
    
//     battery_state_service_unsubscribe();
    
    
    window_destroy(window);
}
