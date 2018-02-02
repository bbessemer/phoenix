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
#include <phoenix/imgfile.h>

#include "memepaths.h"

#define DEGREES     (M_PI / 180.f)
#define BEAT        (60.f / 110.f)
#define MEME_MAX    16
#define STROBE_MAX  16

extern px_image_t hitmarker_image;

static px_tex_t hitmarker_tex;
static px_tex_t meme_texes[13];
static px_tex_t four[20];   // Snoop Dogg GIF (20 frames)

static px_box_t hitmarkers[69];
static px_box_t snoop;
static px_box_t memes[MEME_MAX];
static px_box_t strobes[STROBE_MAX];

#define TTL_MEME_START  0
#define TTL_STROBE_START (TTL_MEME_START + MEME_MAX)
#define TTL_HITMARKER   (TTL_STROBE_START + STROBE_MAX)
#define TTL_SNOOP       (TTL_HITMARKER + 1)
#define N_TTLS          (TTL_SNOOP + 1)
static float ttls[N_TTLS];

void tick ();

void PrintFPS (float fps) {
    printf("%f fps\n", fps);
}

void PrepareTextures () {
    hitmarker_tex = pxMakeTexture(&hitmarker_image);
    for (int i = 0; i < 20; i++) {
        char snoop_path[26];
        sprintf(snoop_path, "crap/snoops/frame_%03d.gif", i);
        px_image_t *snoop_img = pxLoadImage(snoop_path);
        if (!snoop_img) pxFatal("PrepareTextures", snoop_path, __LINE__);
        four[i] = pxMakeTexture(snoop_img);
    }
    for (int i = 0; i < 13; i++) {
        px_image_t *meme_img = pxLoadImage(meme_img_paths[i]);
        if (!meme_img) pxFatal("PrepareTextures", meme_img_paths[i], __LINE__);
        meme_texes[i] = pxMakeTexture(meme_img);
    }
}

/// HITMARKERS

static float respawn_interval = BEAT * 0.25;

void RespawnHitmarker (int i) {
    const float x = 1.8 * (rand() / (float) RAND_MAX - 0.5)
        * pxGetWindowAspect();
    const float y = 1.8 * (rand() / (float) RAND_MAX - 0.5);
    pxSetBoxPos(hitmarkers + i, x, y);
    ttls[TTL_HITMARKER] = respawn_interval;
}

void SpawnHitmarkers () {
    ttls[TTL_HITMARKER] = 10.f; // So none respawn during initial period
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
    RespawnHitmarker(0);
}

void SpinHitmarkers () {
    for (int i = 0; i < 69; i++)
        pxStepRotation(&hitmarkers[i].rotation, (-M_PI / 4.f) * pxGetDelta());
    if (ttls[TTL_HITMARKER] < 0)
        RespawnHitmarker(rand() % 69);
}

/// SNOOP DOGG GIF

int snoop_i = 0;

void SnoopNextFrame () {
    snoop.texture = four[snoop_i++];
    if (snoop_i >= 20) snoop_i = 0;
    ttls[TTL_SNOOP] = 0.1337;
}

void SnoopInit () {
    pxZeroBox(&snoop);
    pxSetBoxDims(&snoop, 0.75f, -0.5f, 1.f, 1.f);
    SnoopNextFrame();
}

/// MEME SPRAY

void ShowRandomMeme (float x, float y) {
    for (int i = 0; i < MEME_MAX; i++) if (memes[i].texture == 0) {
        pxSetBoxDims(memes + i, x, y, 0.3, 0.3);
        memes[i].texture = meme_texes[rand() % 13];
        ttls[TTL_MEME_START + i] = 0.420;
        return;
    }
}

void OnSpray () {
    float x, y;
    pxGetMouse(&x, &y);
    x += 0.64 * (rand() / (float) RAND_MAX - 0.5);
    y += 0.64 * (rand() / (float) RAND_MAX - 0.5);
    ShowRandomMeme(x, y);
}

void OnSnipe () {
    float x, y;
    pxGetMouse(&x, &y);
    ShowRandomMeme(x, y);
}

void MemeInit () {
    for (int i = 0; i < MEME_MAX; i++)
        pxZeroBox(memes + i);
    px_keybind_t *spray, *snipe;
    spray = pxGetKeybind(PX_MOUSE_LEFT, KMOD_NONE);
    snipe = pxGetKeybind(PX_MOUSE_RIGHT, KMOD_NONE);
    spray->repeat_interval = BEAT * 0.125;
    spray->onhold = OnSpray;
    snipe->onpress = OnSnipe;
}

void KillMemes () {
    for (int i = 0; i < MEME_MAX; i++)
        if (ttls[TTL_MEME_START + i] < 0)
            memes[i].texture = 0;
}

/// MAIN LOOP

void tick () {
    pxInputCycle();
    pxTimerCycle(ttls, N_TTLS);

    SpinHitmarkers();
    if (ttls[TTL_SNOOP] < 0) SnoopNextFrame();
    KillMemes();

    pxNewFrame();
    pxDrawBoxes(hitmarkers, 69);
    pxDrawBox(&snoop);
    pxDrawBoxes(memes, MEME_MAX);
}

int main (int argc, char **argv) {
    srand(time(NULL));
    pxRendererInit();
    pxTimerInit();
    PrepareTextures();
    pxCountFPS(PrintFPS, 1000);

    SpawnHitmarkers();
    SnoopInit();
    MemeInit();
    while (!pxGetReqt(PX_REQT_EXIT)) tick();
    return 0;
}
