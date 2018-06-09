/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#pragma once

#include <math.h>
#include <string.h>

#include "texture.h"
#include "color.h"

struct px_rotation {
    float cos, sin;

#ifdef __cplusplus
    inline px_rotation (float theta) : cos(cosf(theta)), sin(sinf(theta)) {}
    inline void step (float dt) { pxStepRotation(this, dt); }
#endif
};

#pragma pack(push, 1)
struct px_box {
    float x, y, w, h;
    struct px_rotation rotation;
    px_color_t color;
    px_tex_t texture;
    unsigned int ent_id;

#ifdef __cplusplus
    inline px_box () { pxZeroBox(this); }
    inline px_box (float x, float y, float w, float h, px_tex_t tex) :
        x(x), y(y), w(w), h(h), texture(tex) {}
    
    inline void setDims (float x, float y, float w, float h) {
        pxSetBoxDims(this, x, y, w, h);
    }

    inline void pxSetBoxPos (float x, float y) { pxSetBoxPos(this, x, y); }
    inline void pxSetBoxSize (float x, float y) { pxSetBoxSize(this, x, y); }
#endif
};
#pragma pack(pop)

typedef struct px_box px_box_t;
typedef struct px_rotation px_rotation_t;

static void pxZeroBox(px_box_t *box) {
    memset(box, 0, sizeof(*box));
    box->rotation.cos = 1;
}

static void pxSetBoxDims (px_box_t *box, float x, float y, float w, float h) {
    box->x = x;
    box->y = y;
    box->w = w;
    box->h = h;
}

static void pxSetBoxSize (px_box_t *box, float w, float h) {
    box->w = w;
    box->h = h;
}

static void pxSetBoxPos (px_box_t *box, float x, float y) {
    box->x = x;
    box->y = y;
}

static void pxSetRotation (px_rotation_t *rot, float theta) {
    *rot = (px_rotation_t) {cosf(theta), sinf(theta)};
}

static void pxStepRotation (px_rotation_t *rot, float dt) {
    float c = rot->cos;
    rot->cos = c * (1 - dt*dt / 2.) - (rot->sin * dt);
    rot->sin = rot->sin * (1 - dt*dt / 2.) + c * dt;
}

static px_box_t *pxFindById (unsigned id, px_box_t *boxes, size_t n_boxes) {
    for (size_t i = 0; i < n_boxes; i++)
        if (boxes[i].ent_id == id) return &boxes[i];
    return NULL;
}
