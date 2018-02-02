/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#pragma once

#include <phoenix/texture.h>

typedef struct px_bmpfont {
    unsigned int char_w, char_h;
    char chars[256];
    // Data follows.
} px_bmpfont_t;

void pxMakeBitmapFont (px_bmpfont_t *font, px_image_t *image,
    const char *string, int char_h);
void pxDrawBitmapChar (px_image_t *out, const px_bmpfont_t *font,
    const size_t i, char c);
void pxRenderTextBitmap (px_image_t *out, const px_bmpfont_t *font,
    const char *string);

extern px_bmpfont_t px_default_font;
