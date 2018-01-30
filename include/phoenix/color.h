/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#pragma once

#include <stdint.h>

typedef struct px_color {
    float r, g, b, a;
} px_color_t;

void pxSetColor_RGBAf (px_color_t *color, float r, float g, float b, float a);
void pxSetColor_RGBAi (px_color_t *color, uint8_t r, uint8_t g, uint8_t b,
        uint8_t a);
void pxSetColor_HSVAf (px_color_t *color, float h, float s, float v, float a);

#define pxSetColor pxSetColor_RGBAf
