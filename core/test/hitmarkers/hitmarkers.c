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
#include <phoenix/ttfont.h>

#include "memepaths.h"

#define DEGREES     (M_PI / 180.f)
#define BEAT        (60.f / 110.f)
#define MEME_MAX    16
#define STROBE_MAX  16
#define FASHION_MAX 16

#define TTL_MEME_START  0
#define TTL_STROBE_START (TTL_MEME_START + MEME_MAX)
#define TTL_FASHION_START (TTL_STROBE_START + STROBE_MAX)
#define TTL_HITMARKER   (TTL_FASHION_START + FASHION_MAX)
#define TTL_SNOOP       (TTL_HITMARKER + 1)
#define TTL_BOXSPAWN    (TTL_SNOOP + 1)
#define N_TTLS          (TTL_BOXSPAWN + 1)

#define SND_BANGARANG   0
#define SND_MEME_START  (SND_BANGARANG + 1)
#define N_MEME_SNDS     8
#define N_SOUNDS        (SND_MEME_START + N_MEME_SNDS)

extern px_image_t hitmarker_image;
extern TTF_Font *px_default_ttf;

static px_tex_t hitmarker_tex;
static px_tex_t meme_texes[13];
static px_tex_t four[20];   // Snoop Dogg GIF (20 frames)

static px_box_t hitmarkers[69];
static px_box_t snoop;
static px_box_t memes[MEME_MAX];
static px_box_t strobes[STROBE_MAX];
static px_box_t fashions[FASHION_MAX];
static float ttls[N_TTLS];
static px_sound_t sounds[N_SOUNDS];

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
        free(snoop_img);
    }
    for (int i = 0; i < 13; i++) {
        px_image_t *meme_img = pxLoadImage(meme_img_paths[i]);
        if (!meme_img) pxFatal("PrepareTextures", meme_img_paths[i], __LINE__);
        meme_texes[i] = pxMakeTexture(meme_img);
        free(meme_img);
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
    PlayMemeSound();
}

void OnSnipe () {
    float x, y;
    pxGetMouse(&x, &y);
    ShowRandomMeme(x, y);
    PlayMemeSound();
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

/// FASHION STATEMENTS

void RandomColor (px_color_t *color) {
    float h = 360.f * (rand() / (float) RAND_MAX);
    pxSetColor_HSVAf(color, h, 0.9f, 1.f, 1.f);
}

void SpawnFashionStatement () {
    const float x = 1.5 * (rand() / (float) RAND_MAX - 0.5)
        * pxGetWindowAspect();
    const float y = 1.5 * (rand() / (float) RAND_MAX - 0.5);
    char *statement = fashion_statements[rand() % 44];
    px_color_t color;
    RandomColor(&color);
    px_image_t *image = pxRenderTextTTF(px_default_ttf, &color, statement);

    for (int i = 0; i < FASHION_MAX; i++) if (fashions[i].texture == 0) {
        pxSetBoxDims(fashions + i, x, y, 0.15 * (image->w / image->h), 0.15);
        pxSetRotation(&fashions[i].rotation,
            (rand() / (float) RAND_MAX - 0.5f) * M_PI / 3.f);
        fashions[i].texture = pxRemakeTexture(fashions[i].texture, image);
        free(image);
        ttls[TTL_FASHION_START + i] = 0.69;
        return;
    }
}

void KillFashionStatements () {
    for (int i = 0; i < FASHION_MAX; i++)
        if (ttls[TTL_FASHION_START + i] < 0)
            fashions[i].texture = 0;
}

/// STROBE BOXES

static float strobe_time = BEAT;
static float maxrot2 = M_PI / 3.;
#define SB_INIT_ALPHA   0.75f

void SpawnStrobeBox () {
    const float aratio = pxGetWindowAspect();
    const float maxlen = 3 * sqrtf(1 + aratio * aratio);
    const float x = (rand() / (float) RAND_MAX - 0.5) * aratio;
    const float y = (rand() / (float) RAND_MAX - 0.5);

    for (int i = 0; i < STROBE_MAX; i++) if (strobes[i].color.a == 0) {
        pxSetBoxDims(strobes + i, x, y, maxlen, 0.15);
        pxSetRotation(&strobes[i].rotation,
            (rand() / (float) RAND_MAX - 0.5f) * maxrot2);
        RandomColor(&strobes[i].color);
        strobes[i].color.a = SB_INIT_ALPHA;
        ttls[TTL_STROBE_START + i] = 0.f;
        return;
    }
}

void KillStrobeBoxes () {
    float deathtime = (strobe_time < BEAT ? 0.1337 : 0.420);
    for (int i = 0; i < STROBE_MAX; i++) {
        float a = SB_INIT_ALPHA * (1 + ttls[TTL_STROBE_START + i] / deathtime);
        strobes[i].color.a = (a > 0 ? a : 0);
    }
}

void SpawnStrobeAndFashion () {
    if (ttls[TTL_BOXSPAWN] < 0) {
        SpawnFashionStatement();
        SpawnStrobeBox();
        ttls[TTL_BOXSPAWN] = strobe_time;
    }
}

/// SOUNDS

#include <sys/stat.h>

void *px_mmap (const char *path) {
    struct stat s;
    stat(path, &s);
    void *buf = malloc(s.st_size);
    FILE *file = fopen(path, "r");
    fread(buf, 1, s.st_size, file);
    return buf;
}

void SoundsInit () {
    memset(sounds, 0, N_SOUNDS * sizeof(px_sound_t));
    pxOpenAudio(2, 96000, 1./60., AUDIO_U16, sounds, N_SOUNDS);
    sounds[SND_BANGARANG].src = px_mmap("crap/bangarang.swag");
    for (int i = 0; i < N_MEME_SNDS; i++)
        sounds[SND_MEME_START + i].src = px_mmap(meme_snd_paths[i]);
}

void PlayMemeSound () {
    int start = rand() % N_MEME_SNDS;
    for (int i = start; i < start + N_MEME_SNDS; i++) {
        int idx = SND_MEME_START + (i % N_MEME_SNDS);
        if (!sounds[idx].playing) {
            sounds[idx].cur_sample = 0;
            sounds[idx].playing = 1;
            return;
        }
    }
}

/// MUSIC SYNC

void vocals ()
{
    maxrot2 = M_PI / 3.f;
    strobe_time = BEAT;
}

void BASS ()
{
    maxrot2 = M_PI / 1.5f;
    strobe_time = 0.25 * BEAT;
}

/// MAIN LOOP

void tick () {
    pxInputCycle();
    pxTimerCycle(ttls, N_TTLS);

    SpinHitmarkers();
    if (ttls[TTL_SNOOP] < 0) SnoopNextFrame();
    SpawnStrobeAndFashion();
    KillMemes();
    KillFashionStatements();
    KillStrobeBoxes();

    pxNewFrame();
    pxDrawBoxes(hitmarkers, 69);
    pxDrawBox(&snoop);
    pxDrawBoxes(strobes, STROBE_MAX);
    pxDrawBoxes(fashions, FASHION_MAX);
    pxDrawBoxes(memes, MEME_MAX);
}

int main (int argc, char **argv) {
    srand(time(NULL));
    pxRendererInit();
    pxTimerInit();
    pxTTFInit("crap/seguisym.ttf");
    PrepareTextures();
    pxCountFPS(PrintFPS, 1000);
    SoundsInit();

    ttls[TTL_BOXSPAWN] = 10.f;
    SpawnHitmarkers();
    SnoopInit();
    MemeInit();
    ttls[TTL_BOXSPAWN] = strobe_time;
    while (!pxGetReqt(PX_REQT_EXIT)) {
        sounds[SND_BANGARANG].playing = 1;
        pxDelay(48 * BEAT, tick);
        if (pxGetReqt(PX_REQT_EXIT)) break;
        BASS();
        pxDelay(64 * BEAT, tick);
        if (pxGetReqt(PX_REQT_EXIT)) break;
        vocals();
        pxDelay(32 * BEAT, tick);
        if (pxGetReqt(PX_REQT_EXIT)) break;
        BASS();
        pxDelay(64 * BEAT, tick);
        if (pxGetReqt(PX_REQT_EXIT)) break;
        vocals();
        pxDelay(32 * BEAT, tick);
        if (pxGetReqt(PX_REQT_EXIT)) break;
        strobe_time = 2 * BEAT;
        pxDelay(32 * BEAT, tick);
        if (pxGetReqt(PX_REQT_EXIT)) break;
        vocals();
        pxDelay(32 * BEAT, tick);
        if (pxGetReqt(PX_REQT_EXIT)) break;
        BASS();
        pxDelay(64 * BEAT, tick);
        vocals();
        while (sounds[SND_BANGARANG].playing && !pxGetReqt(PX_REQT_EXIT))
            tick();
        sounds[SND_BANGARANG].cur_sample = 0;
    }
    return 0;
}
