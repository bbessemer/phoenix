/*
 * hitmarkers v3.2 (MLG Simulator 2006)
 *
 * Code copyright (C) 2017 Brent Bessemer.
 * All rights reserved.
 *
 * Original concept (as "MLG Simulator 2004" and "MLG Simulator 2005")
 * and assets copyright (C) 2014-2017 Seth Carter, if it's even possible to
 * copyright the concept of spraying a bunch of gaming-related logos and memes
 * onto a screen with 69 pixel-art hitmarkers and a black-and-white GIF of
 * Snoop Dogg dancing while shitty dubstep (is there any other kind?) plays
 * in the background.
 *
 * DISCLAIMER: ALL COPYRIGHTED IMAGES, MUSIC, SOUNDS, TRADEMARKS, LIKENESSES,
 * OR OTHER FORMS OF INTELLECTUAL PROPERTY DISPLAYED OR FEATURED HEREIN ARE
 * STRICTLY THE PROPERTY OF THEIR RESPECTIVE OWNERS. AUTHOR HEREBY CLAIMS FAIR
 * USE OF ANY COPYRIGHTED AND/OR TRADEMARKED ASSETS FEATURED HEREIN FOR THE
 * PURPOSE OF PARODY, AND OTHER STRICTLY NON-COMMERCIAL PURPOSES.
 */

#define PX_USE_GL

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <phoenix/phoenix.h>
#include "memepaths.h"

#define DEGREES (M_PI / 180.)

extern px_image_t hitmarker_image;

static px_tex_t hitmarker_tex;
static px_tex_t meme_texes[13];
static px_tex_t four[20];   // Snoop Dogg GIF (20 frames)

static px_box_t hitmarkers[69];
static px_box_t snoop;

void tick ();

void PrintFPS (float fps) {
    printf("%f fps\n", fps);
}

void PrepareTextures () {
    hitmarker_tex = pxMakeTexture(&hitmarker_image);
}

static float beat_time = 0;
static float respawn_interval = 0;

void SpawnHitmarkers () {
    for (int i = 0; i < 69; i++) {
        const float x = 1.8 * (rand() / (float) RAND_MAX - 0.5)
            * pxGetWindowAspect();
        const float y = 1.8 * (rand() / (float) RAND_MAX - 0.5);
        pxZeroBox(hitmarkers + i);
        pxSetBoxDims(hitmarkers + i, x, y, 0.2, 0.2);
        hitmarkers[i].texture = hitmarker_tex;
        pxSetRotation(&hitmarkers[i].rotation,
            1.571 * (rand() / (float) RAND_MAX));
        pxDelay(0.04224306, tick); // 1/69th of the length of the horn sound
    }
}

void RespawnHitmarker (int i) {
    const float x = 1.8 * (rand() / (float) RAND_MAX - 0.5)
        * pxGetWindowAspect();
    const float y = 1.8 * (rand() / (float) RAND_MAX - 0.5);
    pxSetBoxPos(hitmarkers + i, x, y);
}

void SpinHitmarkers () {
    for (int i = 0; i < 69; i++)
        pxStepRotation(&hitmarkers[i].rotation, (-M_PI / 4.f) * pxGetDelta());
    beat_time + pxGetDelta();
    if (respawn_interval && (beat_time > respawn_interval))
        RespawnHitmarker(rand() % 69);
}

///////////////////////////////////////////////////////////

void tick () {
    pxInputCycle();
    pxTimerCycle(NULL, 0);
    SpinHitmarkers();
    pxNewFrame();
    pxDrawBoxes(hitmarkers, 69);
}

int main (int argc, char **argv) {
    pxRendererInit();
    pxTimerInit();
    PrepareTextures();
    SpawnHitmarkers();
    pxCountFPS(PrintFPS, 1000);
    while (!pxGetReqt(PX_REQT_EXIT)) tick();
}
