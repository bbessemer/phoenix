/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#include <phoenix/box.h>

#if defined(PX_COMPILE_GL) || defined(PX_USE_GL)

void pxRendererInit_gl ();
void pxDrawBoxes_gl (px_box_t *boxes, size_t n_boxes);
void pxDrawBox_gl (px_box_t *box);

#endif
