/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#pragma once

#include "color.h"

typedef unsigned int px_tex_t;

typedef struct px_image {
    unsigned int w, h;
    // Pixel data follows
} px_image_t;

px_tex_t pxMakeTexture_gl (px_image_t *img);
px_tex_t pxMakeTexture_Raw_gl (unsigned int w, unsigned int h,
    px_icolor_t *pixels);
px_tex_t pxRemakeTexture_gl (px_tex_t old, px_image_t *new_img);

#ifdef PX_USE_GL

#define pxMakeTexture       pxMakeTexture_gl
#define pxMakeTexture_Raw   pxMakeTexture_Raw_gl
#define pxRemakeTexture     pxRemakeTexture_gl

#endif
