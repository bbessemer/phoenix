/*
 * Copyright (C) 2017 Brent Bessemer.
 * All rights reserved.
 */

#include <phoenix/geometry.h>
#include <math.h>

px::Rotation2D::Rotation2D (float theta) : center(0, 0) {
  cos = cosf(theta);
  sin = sinf(theta);
}

px::Rotation2D::Rotation2D (float theta, float c_x, float c_y) :
  center (c_x, c_y) {
  cos = cosf(theta);
  sin = sinf(theta);
}

void px::Rotation2D::step (float dth) {
  float cos_ = cos;
  cos -= sin * dth;
  sin += cos_ * dth;
}

float px::Rotation2D::angle () {
  return atan2f(sin, cos);
}
