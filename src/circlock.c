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
Layer *hands_layer;
Layer *battery_layer;

Layer *date_layer;
TextLayer *date_label;
static char date_buffer[15];

Layer *time_layer;
TextLayer *time_label;
static char time_buffer[8];

static uint8_t charge_percent = 0;
static bool is_charging = false;

static void date_update_proc(Layer *layer, GContext *ctx)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    strftime(date_buffer, sizeof(date_buffer), "%a, %b %d", t);
    text_layer_set_text(date_label, date_buffer);
}

static void time_update_proc(Layer *layer, GContext *ctx)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    strftime(time_buffer, sizeof(time_buffer), "%I:%M%p", t);
    text_layer_set_text(time_label, time_buffer);
}

static void battery_update_proc(Layer *layer, GContext *ctx)
{
    const uint8_t div = charge_percent / 5;
    const GRect bounds = layer_get_bounds(layer);
    graphics_context_set_fill_color(ctx, CIRCLOCK_COLOR_FOREGROUND);
    GRect frame = (GRect){
        .origin = (GPoint){
            3,
            2 * CIRCLOCK_CLOCK_CENTER_Y + 20
        },
        .size = (GSize){
            bounds.size.w / 20 - 1,
            bounds.size.h
        }
    };
    frame.size.h -= frame.origin.y;
    int8_t i;
    for (i = 0; i < div; ++i)
    {
        graphics_fill_rect(ctx, frame, 4, GCornersAll);
        frame.origin.x += frame.size.w + 1;
    }
}

static void handle_battery(BatteryChargeState charge_state)
{
    charge_percent = charge_state.charge_percent;
    is_charging = charge_state.is_charging;
    layer_set_update_proc(battery_layer, battery_update_proc);
}

static void handle_second_tick(struct tm *tick_time, TimeUnits units_changed)
{
    handle_battery(battery_state_service_peek());
    layer_mark_dirty(window_get_root_layer(window));
}

static void window_load(Window *window)
{
    Layer *window_layer = window_get_root_layer(window);
    const GRect bounds = layer_get_bounds(window_layer);
    GPoint center = grect_center_point(&bounds);

    // init layers
    bg_layer = layer_create(bounds);
    layer_set_update_proc(bg_layer, circlock_bg_update_proc);
    layer_add_child(window_layer, bg_layer);
    
    // init date layer -> a plain parent layer to create a date update proc
    date_layer = layer_create(bounds);
    layer_set_update_proc(date_layer, date_update_proc);
    layer_add_child(window_layer, date_layer);

    // init date
    const int16_t date_label_width = 80;
    date_label = text_layer_create(GRect(center.x - date_label_width / 2, 2 * CIRCLOCK_CLOCK_CENTER_Y + 2, date_label_width, 20));
    text_layer_set_text(date_label, date_buffer);
    text_layer_set_background_color(date_label, GColorClear);
    text_layer_set_text_color(date_label, CIRCLOCK_COLOR_FOREGROUND);
    GFont norm18 = fonts_get_system_font(FONT_KEY_GOTHIC_14);
    text_layer_set_font(date_label, norm18);
    text_layer_set_text_alignment(date_label, GTextAlignmentCenter);
    layer_add_child(date_layer, text_layer_get_layer(date_label));
    
    // init time layer -> a plain parent layer to create a date update proc
    time_layer = layer_create(bounds);
    layer_set_update_proc(time_layer, time_update_proc);
    layer_add_child(window_layer, time_layer);

    // init time
    time_label = text_layer_create(GRect(10, CIRCLOCK_CLOCK_CENTER_Y - 10, bounds.size.w - 20, 20));
    text_layer_set_text(time_label, time_buffer);
    text_layer_set_background_color(time_label, GColorClear);
    text_layer_set_text_color(time_label, CIRCLOCK_COLOR_FOREGROUND);
    GFont norm14 = fonts_get_system_font(FONT_KEY_GOTHIC_18);
    text_layer_set_font(time_label, norm14);
    text_layer_set_text_alignment(time_label, GTextAlignmentCenter);
    layer_add_child(time_layer, text_layer_get_layer(time_label));

    // init hands
    hands_layer = layer_create(bounds);
    layer_set_update_proc(hands_layer, circlock_hands_update_proc);
    layer_add_child(window_layer, hands_layer);
    
    // init battery
    battery_layer = layer_create(bounds);
    layer_set_update_proc(battery_layer, battery_update_proc);
    layer_add_child(window_layer, battery_layer);
}

static void window_unload(Window *window)
{
    layer_destroy(battery_layer);
    layer_destroy(hands_layer);
    text_layer_destroy(time_label);
    layer_destroy(time_layer);
    text_layer_destroy(date_label);
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
    
    date_buffer[0] = '\0';
    time_buffer[0] = '\0';
    
    circlock_bg_init();
    circlock_hands_init(window_get_root_layer(window));
    
    const bool animated = true;
    window_stack_push(window, animated);

    tick_timer_service_subscribe(SECOND_UNIT, &handle_second_tick);
    battery_state_service_subscribe(&handle_battery);
}

void circlock_deinit()
{
    battery_state_service_unsubscribe();
    tick_timer_service_unsubscribe();
    circlock_hands_deinit();
    circlock_bg_deinit();
    
    window_destroy(window);
}
