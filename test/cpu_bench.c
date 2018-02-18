#include <phoenix/phoenix.h>

int main (void) {
    SDL_Init(0);
    pxRendererInit();
    pxTimerInit();
    px_box_t box;
    px_color_t color;
    float ttl = 0;
    float x = 0.4, y = 0.6;
    while (1) {
        pxSetBoxDims(&box, 0.1, 0.2, 0.3, 0.4);
        pxSetBoxPos(&box, 0.69, 0.69);
        pxSetBoxSize(&box, 0.420, 0.420);
        pxSetRotation(&box.rotation, 1.23);
        pxStepRotation(&box.rotation, 0.01);
        pxSetColor_RGBAf(&color, 0.5, 0.5, 0.5, 1.);
        pxSetColor_RGBAi(&color, 221, 168, 42, 255);
        pxSetColor_HSVAf(&color, 72., 0.6, 0.5, 0.9);
        pxRefreshCamera();
        pxScreentoWorld(&x, &y);
        pxGetWindowAspect();
        pxTimerCycle(&ttl, 1);
        pxGetDelta();
        pxGetFrameCount();
        pxGetGameTime();
    }
}
