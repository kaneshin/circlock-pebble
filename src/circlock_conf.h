// circlock_conf.h
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

#pragma once

#define CIRCLOCK_CLOCK_RADIUS 64

#define CIRCLOCK_HAND_WIDTH 8
#define CIRCLOCK_HAND_HEIGHT 4
#define CIRCLOCK_HAND_MARGIN 1

#define CIRCLOCK_CLOCK_CENTER_Y (CIRCLOCK_CLOCK_RADIUS + CIRCLOCK_HAND_HEIGHT)

#define CIRCLOCK_INVERT_COLORS 0
#if defined(CIRCLOCK_INVERT_COLORS) && CIRCLOCK_INVERT_COLORS == 1
#define CIRCLOCK_COLOR_FOREGROUND GColorBlack
#define CIRCLOCK_COLOR_BACKGROUND GColorWhite
#else
#define CIRCLOCK_COLOR_FOREGROUND GColorWhite
#define CIRCLOCK_COLOR_BACKGROUND GColorBlack
#endif
