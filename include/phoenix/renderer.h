/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#pragma once

#include <phoenix/box.h>

#define PX_CAM_NOCHANGE 0
#define PX_CAM_STRETCH  1
#define PX_CAM_OVERFLOW 2
#define PX_CAM_FIT      3

px_color_t *pxBackgroundColor ();
void pxRefreshCamera ();
float pxGetWindowAspect ();
void pxSetCamera (float x, float y, float w, float h, int mode);
void pxScreentoWorld (float *_x, float *_y);
void pxRendererInit ();

#if defined(PX_COMPILE_GL) || defined(PX_USE_GL)

void pxNewFrame_gl ();
void pxRendererInit_gl ();
void pxDrawBoxes_gl (px_box_t *boxes, size_t n_boxes);
void pxDrawBox_gl (px_box_t *box);

#endif

#ifdef PX_USE_GL

#define pxNewFrame      pxNewFrame_gl
#define pxRendererInit  pxRendererInit_gl
#define pxDrawBoxes     pxDrawBoxes_gl
#define pxDrawBox       pxDrawBox_gl

#endif
