/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#pragma once

#include <SDL2/SDL_ttf.h>

void pxTTFInit (const char *dflt_fontpath);
px_image_t *pxRenderTextTTF (TTF_Font *font, px_color_t *color,
    const char *str);
