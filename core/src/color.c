/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#include <math.h>
#include <phoenix/color.h>

void pxSetColor_RGBAf (px_color_t *color, float r, float g, float b, float a) {
    *color = (px_color_t) {r, g, b, a};
}

void pxSetColor_RGBAi (px_color_t *color, uint8_t r, uint8_t g, uint8_t b,
        uint8_t a)
{
    *color = (px_color_t) {
        r / 255.0,
        g / 255.0,
        b / 255.0,
        a / 255.0
    };
}

void pxSetColor_HSVAf (px_color_t *color, float h, float s, float v, float a) {
    register float r, g, b;

    register const float chroma = v * s;
    h = fmodf(h, 360.) / 60.;
    register const float x = chroma * (1 - fabsf(fmodf(h, 2.) - 1));
    switch ((int)h)
    {
    case 0:
        r = chroma;
        g = x;
        b = 0;
        break;
    case 1:
        r = x;
        g = chroma;
        b = 0;
        break;
    case 2:
        r = 0;
        g = chroma;
        b = x;
        break;
    case 3:
        r = 0;
        g = x;
        b = chroma;
        break;
    case 4:
        r = x;
        g = 0;
        b = chroma;
        break;
    case 5:
        r = chroma;
        g = 0;
        b = x;
        break;
    default:
        break;
    }

    register const float m = v - chroma;
    *color = (px_color_t) {r, g, b, a};
}
