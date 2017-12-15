/*
 * Copyright (C) 2017 Brent Bessemer.
 * All rights reserved.
 */

/**
 *
 */

#pragma once

namespace px {

struct Point2D {
  float x, y;
  Point2D (float x, float y) : x(x), y(y) {}
};

struct Rotation2D {
  float sin, cos;
  Point2D center;

  Rotation2D (float theta);
  Rotation2D (float theta, float c_x, float c_y);
  void step (float dth);
  float angle ();
};

}
