/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#include <string.h>
#include <math.h>
#include <phoenix/box.h>
/*
void pxZeroBox(px_box_t *box) {
    memset(box, 0, sizeof(*box));
    box->rotation.cos = 1;
}

void pxSetBoxDims (px_box_t *box, float x, float y, float w, float h) {
    box->x = x;
    box->y = y;
    box->w = w;
    box->h = h;
}

void pxSetBoxSize (px_box_t *box, float w, float h) {
    box->w = w;
    box->h = h;
}

void pxSetBoxPos (px_box_t *box, float x, float y) {
    box->x = x;
    box->y = y;
}
*/
void pxSetRotation (px_rotation_t *rot, float theta) {
    *rot = (px_rotation_t) {cosf(theta), sinf(theta), 0.f, 0.f};
}
/*
void pxStepRotation (px_rotation_t *rot, float dt) {
    float c = rot->cos;
    rot->cos = c * (1 - dt*dt) - (rot->sin * dt);
    rot->sin = rot->sin * (1 - dt*dt) + c * dt;
}
*/
