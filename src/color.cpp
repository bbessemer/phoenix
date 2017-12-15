/*
 * Copyright (C) 2017 Brent Bessemer.
 * All rights reserved.
 */

#include <phoenix/color.h>

px::RGBAColor
px::RGBAColor::FromHSVA (float h, const float s, const float v, const float a)
{
  float r, g, b;

  const float chroma = v * s;
  h = fmodf(h, 360.) / 60.;
  const float x = chroma * (1 - fabsf(fmodf(h, 2.) - 1));
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

  const float m = v - chroma;

  return RGBAColor(r + m, g + m, b + m, a);
}
