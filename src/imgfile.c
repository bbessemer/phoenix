/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#ifndef PX_NO_IMGFILE

#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <phoenix/imgfile.h>

px_image_t *pxLoadImage (const char *path) {
    SDL_Surface *surf, *conv;
    int w, h;
    size_t size;
    px_image_t *image;

    surf = IMG_Load(path);
    if (!surf) {
        printf("Cannot load image %s\n", path);
        return NULL;
    }
    conv = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_ABGR8888, 0);
    if (!conv) return NULL;
    SDL_FreeSurface(surf);
    w = conv->w;
    h = conv->h;
    size = w * h * 4;
    image = malloc(sizeof(px_image_t) + size);
    memcpy(image + 1, conv->pixels, size);
    SDL_FreeSurface(conv);

    image->w = w;
    image->h = h;
    return image;
}

#endif
