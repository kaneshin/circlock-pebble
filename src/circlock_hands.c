// circlock_hands.c
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

#include "circlock_hands.h"

#include "circlock_conf.h"

static GPath *second_hand;
static GPath *minute_hand;
static GPath *hour_hand;

#define CIRCLOCK_SECOND_HAND_POINT_Y (CIRCLOCK_CLOCK_RADIUS + CIRCLOCK_HAND_MARGIN)
static const GPathInfo SECOND_HAND_POINTS =
{
    .num_points = 4,
    .points = (GPoint []) {
        {-CIRCLOCK_HAND_WIDTH / 2, CIRCLOCK_SECOND_HAND_POINT_Y + CIRCLOCK_HAND_MARGIN},
        { CIRCLOCK_HAND_WIDTH / 2, CIRCLOCK_SECOND_HAND_POINT_Y + CIRCLOCK_HAND_MARGIN},
        { CIRCLOCK_HAND_WIDTH / 2, CIRCLOCK_SECOND_HAND_POINT_Y - CIRCLOCK_HAND_HEIGHT - CIRCLOCK_HAND_MARGIN},
        {-CIRCLOCK_HAND_WIDTH / 2, CIRCLOCK_SECOND_HAND_POINT_Y - CIRCLOCK_HAND_HEIGHT - CIRCLOCK_HAND_MARGIN}
    }
};

#define CIRCLOCK_MINUTE_HAND_POINT_Y (CIRCLOCK_SECOND_HAND_POINT_Y - CIRCLOCK_HAND_HEIGHT - CIRCLOCK_HAND_MARGIN)
static const GPathInfo MINUTE_HAND_POINTS =
{
    .num_points = 4,
    .points = (GPoint []) {
        {-CIRCLOCK_HAND_WIDTH / 2, CIRCLOCK_MINUTE_HAND_POINT_Y + CIRCLOCK_HAND_MARGIN},
        { CIRCLOCK_HAND_WIDTH / 2, CIRCLOCK_MINUTE_HAND_POINT_Y + CIRCLOCK_HAND_MARGIN},
        { CIRCLOCK_HAND_WIDTH / 2, CIRCLOCK_MINUTE_HAND_POINT_Y - CIRCLOCK_HAND_HEIGHT - CIRCLOCK_HAND_MARGIN},
        {-CIRCLOCK_HAND_WIDTH / 2, CIRCLOCK_MINUTE_HAND_POINT_Y - CIRCLOCK_HAND_HEIGHT - CIRCLOCK_HAND_MARGIN}
    }
};

#define CIRCLOCK_HOUR_HAND_POINT_Y (CIRCLOCK_MINUTE_HAND_POINT_Y - CIRCLOCK_HAND_HEIGHT - CIRCLOCK_HAND_MARGIN)
static const GPathInfo HOUR_HAND_POINTS = 
{
    .num_points = 4,
    .points = (GPoint []) {
        {-CIRCLOCK_HAND_WIDTH / 2, CIRCLOCK_HOUR_HAND_POINT_Y + CIRCLOCK_HAND_MARGIN},
        { CIRCLOCK_HAND_WIDTH / 2, CIRCLOCK_HOUR_HAND_POINT_Y + CIRCLOCK_HAND_MARGIN},
        { CIRCLOCK_HAND_WIDTH / 2, CIRCLOCK_HOUR_HAND_POINT_Y - CIRCLOCK_HAND_HEIGHT - CIRCLOCK_HAND_MARGIN},
        {-CIRCLOCK_HAND_WIDTH / 2, CIRCLOCK_HOUR_HAND_POINT_Y - CIRCLOCK_HAND_HEIGHT - CIRCLOCK_HAND_MARGIN}
    }
};

void circlock_hands_update_proc(Layer *layer, GContext *ctx)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    // second/minute/hour hands
    gpath_rotate_to(second_hand, TRIG_MAX_ANGLE * (t->tm_sec + 30) / 60);
    gpath_rotate_to(minute_hand, TRIG_MAX_ANGLE * (t->tm_min + 30) / 60);
    gpath_rotate_to(hour_hand, (TRIG_MAX_ANGLE * (((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6) + TRIG_MAX_ANGLE / 2);
    
    graphics_context_set_fill_color(ctx, CIRCLOCK_COLOR_BACKGROUND);
    gpath_draw_filled(ctx, second_hand);
    gpath_draw_filled(ctx, minute_hand);
    gpath_draw_filled(ctx, hour_hand);
    
//     graphics_context_set_stroke_color(ctx, CIRCLOCK_COLOR_FOREGROUND);
//     gpath_draw_outline(ctx, second_hand);
//     gpath_draw_outline(ctx, minute_hand);
//     gpath_draw_outline(ctx, hour_hand);
}

void circlock_hands_init(Layer *layer)
{
    const GRect bounds = layer_get_bounds(layer);
    GPoint center = grect_center_point(&bounds);
    center.y = CIRCLOCK_CLOCK_CENTER_Y + CIRCLOCK_HAND_HEIGHT - CIRCLOCK_HAND_HEIGHT;
    
    second_hand = gpath_create(&SECOND_HAND_POINTS);
    minute_hand = gpath_create(&MINUTE_HAND_POINTS);
    hour_hand = gpath_create(&HOUR_HAND_POINTS);
    
    gpath_move_to(second_hand, center);
    gpath_move_to(minute_hand, center);
    gpath_move_to(hour_hand, center);
}

void circlock_hands_deinit()
{
    gpath_destroy(second_hand);
    gpath_destroy(minute_hand);
    gpath_destroy(hour_hand);
}
