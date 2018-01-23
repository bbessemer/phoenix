/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#pragma once

#include "texture.h"
#include "color.h"

struct px_rotation {
    float cx, cy;
    float cos, sin;
};

struct px_box {
    unsigned int ent_id;
    float x, y, w, h;
    struct px_rotation rotation;
    px_tex_t texture;
    px_color_t color;
};

typedef struct px_box px_box_t;
typedef struct px_rotation px_rotation_t;

void pxSetBoxDims (px_box_t *box, float x, float y, float w, float h);
void pxSetBoxSize (px_box_t *box, float w, float h);
void pxSetBoxPos (px_box_t *box, float x, float y);

void pxSetRotation (px_rotation_t *rot, float theta);
void pxStepRotation (px_rotation_t *rot, float dt);
