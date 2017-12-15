/*
 * Copyright (C) 2017 Brent Bessemer.
 * All rights reserved.
 */

/**
 *
 */

#pragma once

#include <SDL2/SDL.h>

namespace px {

#pragma pack(1)
struct RGBAColor {
  float r, g, b, a;

  inline RGBAColor (float r, float g, float b, float a) :
    r(r), g(g), b(b), a(a) {}

  inline RGBAColor (uint8_t r, uint8_t g, uint8_t b, uint8_t a) :
    r(r * (1.f / 255.f)),
    g(g * (1.f / 255.f)),
    b(b * (1.f / 255.f)),
    a(a * (1.f / 255.f)) {}

  inline SDL_Color toSDL_Color () {
    SDL_Color out;
    out.r = r * 0xff;
    out.g = g * 0xff;
    out.b = b * 0xff;
    out.a = a * 0xff;
    return out;
  }

  static RGBAColor FromHSVA (float h, const float s, const float v,
    const float a);
};

}
