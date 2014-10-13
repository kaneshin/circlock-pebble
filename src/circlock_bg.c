// circlock_bg.c
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

#include "circlock_bg.h"

#include "circlock_conf.h"

void draw_fill_circle(GContext *ctx, GPoint center, uint16_t *radius)
{
    graphics_context_set_fill_color(ctx, CIRCLOCK_COLOR_FOREGROUND);
    graphics_fill_circle(ctx, center, *radius);
    
    *radius -= CIRCLOCK_HAND_HEIGHT;
    graphics_context_set_fill_color(ctx, CIRCLOCK_COLOR_BACKGROUND);
    graphics_fill_circle(ctx, center, *radius);
    
    *radius -= CIRCLOCK_HAND_MARGIN;
}

void circlock_bg_update_proc(Layer *layer, GContext *ctx)
{
    graphics_context_set_fill_color(ctx, CIRCLOCK_COLOR_BACKGROUND);
    graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
    
    const GRect bounds = layer_get_bounds(layer);
    GPoint center = grect_center_point(&bounds);
    center.y = CIRCLOCK_CLOCK_CENTER_Y;
    
    uint16_t radius = CIRCLOCK_CLOCK_RADIUS;
    draw_fill_circle(ctx, center, &radius);
    draw_fill_circle(ctx, center, &radius);
    draw_fill_circle(ctx, center, &radius);
    
    graphics_context_set_stroke_color(ctx, CIRCLOCK_COLOR_FOREGROUND);
    graphics_draw_line(ctx, (GPoint){10, 2 * CIRCLOCK_CLOCK_CENTER_Y}, (GPoint){bounds.size.w - 10, 2 * CIRCLOCK_CLOCK_CENTER_Y});
}

void circlock_bg_init()
{
}

void circlock_bg_deinit()
{
}
    