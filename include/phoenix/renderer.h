/*
 * Copyright (C) 2017 Brent Bessemer.
 * All rights reserved.
 */

/**
 *
 */

#pragma once

#include <SDL2/SDL.h>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <glad/glad.h>
#endif

#include "geometry.h"
#include "container.h"
#include "box.h"

namespace px {

enum CameraMode {
  PX_CAM_NOCHANGE = 0,
  PX_CAM_STRETCH,
  PX_CAM_OVERFLOW,
  PX_CAM_FIT
};

class Renderer2D {
public:
  ISize2D window_size;

  Renderer2D ();
  ~Renderer2D ();
  void setCamera (float, float, float, float, CameraMode);

  inline void refreshCamera () {
    setCamera(cam_center.x, cam_center.y, cam_size.w, cam_size.h,
      PX_CAM_NOCHANGE);
  }

  inline float getWindowAspect () {
    return float(window_size.w) / float(window_size.h);
  }

  void screenToWorld (Point2D &pos);

private:
  Size2D cam_size;
  Point2D cam_center;
  CameraMode cam_mode;
  float cam_matrix[4][4];

public:
  void cycle ();
  void drawBoxes (Container<Box2D> boxes);

private:
  SDL_GLContext gl_context;
};

}
