/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#include <string.h>
#include <phoenix/renderer.h>

float cam_center_x = 0, cam_center_y = 0;
float cam_size_w = 2, cam_size_h = 2;
int cam_mode = PX_CAM_FIT;

float camera_matrix[4][4];
unsigned int window_size_w = 1024;
unsigned int window_size_h = 768;

px_color_t px_background_color = {0, 0, 0, 1};

px_color_t *pxBackgroundColor () { return &px_background_color; }

void pxRendererInit () {
    memset(&camera_matrix[0][1], 0, sizeof(float) * 14);
    camera_matrix[3][3] = 1.f;
    pxRefreshCamera();
}

void pxRefreshCamera () {
    pxSetCamera(cam_center_x, cam_center_y, cam_size_w, cam_size_h,
        PX_CAM_NOCHANGE);
}

float pxGetWindowAspect () {
    return window_size_w / (float) window_size_h;
}

void pxSetCamera (float x, float y, float w, float h, int mode) {
    if (mode == PX_CAM_NOCHANGE)
        mode = cam_mode;

    cam_center_x = x;
    cam_center_y = y;
    cam_size_w = w;
    cam_size_h = h;
    cam_mode = mode;

    float win_aratio = pxGetWindowAspect();
    float cam_aratio = w / h;
    switch (mode) {
    case PX_CAM_FIT:
    case PX_CAM_OVERFLOW:
        if ((win_aratio > cam_aratio) != (mode == PX_CAM_FIT))
            h = w / win_aratio;
        else
            w = h * win_aratio;
        break;
    case PX_CAM_STRETCH:
        break;
    default:
        return;
    }

    camera_matrix[0][0] = 1.f / w;
    camera_matrix[1][1] = 1.f / h;
    camera_matrix[3][0] = -x * 0.5f / w;
    camera_matrix[3][1] = -y * 0.5f / h;
}

void pxScreentoWorld (float *_x, float *_y) {
    float x = *_x - 0.5;
    float y = *_y - 0.5;
    *_x =  x / camera_matrix[0][0] + cam_center_x;
    *_y = -y / camera_matrix[1][1] + cam_center_y;
}
