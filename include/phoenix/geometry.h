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

struct Size2D {
  float w, h;
  Size2D (float w, float h) : w(w), h(h) {}
};

struct IPoint2D {
  int x, y;
  IPoint2D (int x, int y) : x(x), y(y) {}
};

struct ISize2D {
  unsigned int w, h;
  ISize2D (unsigned int w, unsigned int h) : w(w), h(h) {}
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
