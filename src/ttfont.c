/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#include <phoenix/color.h>
#include <phoenix/texture.h>
#include <phoenix/ttfont.h>
#include <phoenix/error.h>

TTF_Font *px_default_ttf;

void pxTTFInit (const char *dflt_fontpath) {
    TTF_Init();
    px_default_ttf = TTF_OpenFont(dflt_fontpath, 64);
    if (!px_default_ttf)
        pxFatal("TTFLoader", "Could not find default font", __LINE__);
}

px_image_t *pxRenderTextTTF (TTF_Font *font, px_color_t *color, const char *str)
{
    SDL_Color sdl_color;
    if (color) {
        sdl_color = (SDL_Color){
            .r = color->r * 0xff,
            .g = color->g * 0xff,
            .b = color->b * 0xff,
            .a = color->a * 0xff
        };
    } else {
        sdl_color = (SDL_Color){255, 255, 255, 255};
    }
    SDL_Surface *orig = TTF_RenderText_Blended(px_default_ttf, str, sdl_color);
    if (!orig) return NULL;
    SDL_Surface *conv = SDL_ConvertSurfaceFormat(orig, SDL_PIXELFORMAT_ABGR8888, 0);
    if (!conv) return NULL;
    unsigned int w = conv->w;
    unsigned int h = conv->h;
    size_t size = w * h * 4;
    px_image_t *image = malloc(sizeof(px_image_t) + size);
    memcpy(image + 1, conv->pixels, size);
    SDL_FreeSurface(conv);
    SDL_FreeSurface(orig);

    *image = (px_image_t){w, h};
    return image;
}
