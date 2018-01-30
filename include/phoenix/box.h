/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#pragma once

#include "texture.h"
#include "color.h"

struct px_rotation {
    float cos, sin;
    float cx, cy;
};

struct px_box {
    float x, y, w, h;
    struct px_rotation rotation;
    px_color_t color;
    px_tex_t texture;
    unsigned int ent_id;
};

typedef struct px_box px_box_t;
typedef struct px_rotation px_rotation_t;

void pxZeroBox(px_box_t *box);

void pxSetBoxDims (px_box_t *box, float x, float y, float w, float h);
void pxSetBoxSize (px_box_t *box, float w, float h);
void pxSetBoxPos (px_box_t *box, float x, float y);

void pxSetRotation (px_rotation_t *rot, float theta);
void pxStepRotation (px_rotation_t *rot, float dt);
