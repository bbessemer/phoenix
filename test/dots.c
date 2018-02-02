/*
 * Eat Dots
 *
 * Concept and artwork copyright (C) 2014, Nathan Estock and Craig Rhodes.
 * Code copyright (C) 2018, Brent Bessemer.
 * All rights reserved.
 */

#define PX_USE_GL

#include <stdlib.h>
#include <time.h>
#include <phoenix/phoenix.h>

static px_icolor_t wamba_tex[8][8];
static px_icolor_t food_tex[6][6];
static px_icolor_t poison_tex[6][6];

void MakeTextures () {
    // Wamba
    for (int i = 0; i < 64; i++)
        wamba_tex[0][i] = (px_icolor_t){11, 123, 255, 255}; // Background
    for (int i = 1; i <= 3; i++) {
        wamba_tex[i][2] = (px_icolor_t){0, 0, 0, 255}; // Left eye
        wamba_tex[i][5] = (px_icolor_t){0, 0, 0, 255}; // Right eye
    }

    // Food
    for (int i = 0; i < 36; i++)
        food_tex[0][i] = (px_icolor_t){0, 249, 0, 255};
    food_tex[1][1] = (px_icolor_t){0, 0, 0, 255};
    food_tex[1][4] = (px_icolor_t){0, 0, 0, 255};

    // Poison
    for (int i = 0; i < 36; i++)
        poison_tex[0][i] = (px_icolor_t){235, 61, 0, 255};
    poison_tex[1][1] = (px_icolor_t){0, 0, 0, 255};
    poison_tex[1][4] = (px_icolor_t){0, 0, 0, 255};
}

#define ROUND_TIME 1.6
static px_box_t character, food, poison;
static float ttl;
static int score;

#pragma pack(1)
static struct {
    px_bmpfont_t font;
    px_icolor_t data[320][5];
} default_font;

#pragma pack(1)
static struct {
    px_image_t header;
    px_icolor_t data[5][15];
} score_tex;

#pragma pack(1)
static struct {
    px_image_t header;
    px_icolor_t data[5][20];
} fps_tex;

static px_box_t score_disp;
static px_box_t fps_disp;

void RedrawScore () {
    char score_str[4];
    snprintf(score_str, 3, "%d", score);
    pxRenderTextBitmap(&score_tex.header, &default_font.font, score_str);
    score_disp.texture = pxRemakeTexture_gl(score_disp.texture,
        &score_tex.header);
}

void RedrawFPS (float fps) {
    if (fps > 10000) fps = 9999;
    char fps_string[5];
    snprintf(fps_string, 5, "%d", (int) fps);
    pxRenderTextBitmap(&fps_tex.header, &default_font.font, fps_string);
    fps_disp.texture = pxRemakeTexture_gl(fps_disp.texture, &fps_tex.header);
}

char RectCheck (px_box_t* box0, px_box_t* box1, void (*oncollide) ()) {
    float max_x = (box0->w + box1->w) * 0.5f;
    float diff_x = fabsf(box0->x - box1->x);
    if (diff_x >= max_x) return 0;

    float max_y = (box0->h + box1->h) * 0.5f;
    float diff_y = fabsf(box0->y - box1->y);
    if (diff_y >= max_y) return 0;

    oncollide();
    return 1;
}

void NewPositions () {
    ttl = ROUND_TIME;

    pxSetBoxPos(&food, rand() / (float) RAND_MAX * 1.5f - 1.f,
        rand() / (float) RAND_MAX * 1.5f - 1.f);
    pxSetBoxPos(&poison, rand() / (float) RAND_MAX * 1.5 - 1.f,
        rand() / (float) RAND_MAX * 1.5f - 1.f);
    pxSetBoxSize(&character, 0.3 + score * 0.04, 0.3 + score * 0.04);

    RectCheck(&character, &food, NewPositions);
    RectCheck(&character, &poison, NewPositions);
}

void OnEatFood () {
    score++;
    NewPositions();
    RedrawScore();
}

void OnEatPoison () {
    score = 0;
    NewPositions();
    RedrawScore();
}

void WambaCheck ()
{
    if (RectCheck(&character,&food,OnEatFood)) return;
    RectCheck(&character,&poison,OnEatPoison);
    if (ttl < 0 && score) OnEatPoison();
}

int main ()
{
	srand(time(NULL));

    pxZeroBox(&character);
    pxZeroBox(&food);
    pxZeroBox(&poison);
    pxZeroBox(&score_disp);
    pxZeroBox(&fps_disp);

    MakeTextures();
    pxMakeDefaultFont(&default_font.font);

    pxRendererInit();

    character.texture = pxMakeTexture_Raw_gl(8, 8, (px_icolor_t *)wamba_tex);
    food.texture = pxMakeTexture_Raw_gl(6, 6, (px_icolor_t *)food_tex);
    poison.texture = pxMakeTexture_Raw_gl(6, 6, (px_icolor_t *)poison_tex);

    score_tex.header = (px_image_t){.w = 15, .h = 5};
    fps_tex.header = (px_image_t){.w = 20, .h = 5};

    pxSetBoxSize(&food, 0.1, 0.1);
    pxSetBoxSize(&poison, 0.1, 0.1);
    pxSetBoxDims(&score_disp, 0.9, -0.9, 0.45, 0.15);
    pxSetBoxDims(&fps_disp, 0.9, 0.9, 0.4, 0.1);

    pxSetColor(pxBackgroundColor(), 1, 1, 1, 1);
    pxTimerInit();
    //SDL_GL_SetSwapInterval(1); // Turn on vsync so as to not murder my battery
    pxCountFPS(RedrawFPS, 1000);

    OnEatPoison(); // starts new game

    while (!pxGetReqt(PX_REQT_EXIT)) {
        pxInputCycle();
        pxTimerCycle(&ttl, 1);

        pxGetMouse(&character.x, &character.y);

        WambaCheck();

        pxNewFrame();
        pxDrawBox(&food);
        pxDrawBox(&poison);
        pxDrawBox(&character);
        pxDrawBox(&score_disp);
        pxDrawBox(&fps_disp);
    }
}
