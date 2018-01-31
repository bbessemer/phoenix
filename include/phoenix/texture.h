#pragma once

#include "color.h"

typedef unsigned int px_tex_t;

typedef struct px_image {
    unsigned int w, h;
    px_icolor_t *pixels;
} px_image_t;

px_tex_t pxMakeTexture_gl (px_image_t *img);
px_tex_t pxMakeTexture_Raw_gl (unsigned int w, unsigned int h,
    px_icolor_t *pixels);
