/*
 * Copyright (C) 2017 Brent Bessemer.
 * All rights reserved.
 */

/**
 *
 */

#pragma once

#include "entity.h"
#include "color.h"
#include "geometry.h"

namespace px {

class Box2D : public Entity {
public:
  float x, y, w, h;
  Rotation2D rotation;
  RGBAColor color;
  // TODO Texture tex;

  Box2D (float x, float y, float w, float h) :
    Entity(),
    x(x), y(y), w(w), h(h),
    rotation(0),
    color(0.f, 0.f, 0.f, 0.f) {}

  Box2D (float x, float y, float w, float h, float theta, float r_x, float r_y,
    float r, float g, float b, float a) :
    Entity(),
    x(x), y(y), w(w), h(h),
    rotation(theta, r_x, r_y),
    color(r, g, b, a) {}

  inline void setDims (float x_n, float y_n, float w_n, float h_n) {
    x = x_n;
    y = y_n;
    w = w_n;
    h = h_n;
  }
};

}
