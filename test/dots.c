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

static px_color_t wamba_tex[8][8];
static px_color_t food_tex[6][6];
static px_color_t poison_tex[6][6];

void MakeTextures () {
    // Wamba
    for (int i = 0; i < 64; i++)
        pxSetColor_RGBAi(wamba_tex + i, 11, 123, 255, 255); // Background
    for (int i = 1; i <= 3; i++) {
        wamba_tex[i][2] = (px_color_t){0, 0, 0, 1}; // Left eye
        wamba_tex[i][5] = (px_color_t){0, 0, 0, 1}; // Right eye
    }

    // Food
    for (int i = 0; i < 36; i++)
        pxSetColor_RGBAi(food_tex + i, 235, 61, 0, 255);
    food_tex[1][1] = (px_color_t){0, 0, 0, 1};
    food_tex[1][4] = (px_color_t){0, 0, 0, 1};

    // Poison
    for (int i = 0; i < 36; i++)
        pxSetColor_RGBAi(poison_tex + i, 0, 249, 0, 255);
    poison_tex[1][1] = (px_color_t){0, 0, 0, 1};
    poison_tex[1][4] = (px_color_t){0, 0, 0, 1};
}

#define ROUND_TIME 1.6
static px_box_t character, food, poison;
static float ttl;
static int score;

void NewPositions () {
    ttl = ROUND_TIME;

    pxSetBoxPos(&food, rand() / (float) RAND_MAX * 1.5f - 1.f,
        rand() / (float) RAND_MAX * 1.5f - 1.f);
    pxSetBoxPos(&poison, rand() / (float) RAND_MAX * 1.5 - 1.f,
        rand() / (float) RAND_MAX * 1.5f - 1.f);
}

void OnEatFood () {
    score++;
    NewPositions();
}

void OnEatPoison () {
    ttl = ROUND_TIME;
    NewPositions();
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

void WambaCheck ()
{
    if (RectCheck(&character,&food,OnEatFood)) return;
    RectCheck(&character,&poison,OnEatPoison);
}

int main ()
{
	srand(time(NULL));

    pxZeroBox(&character);
    pxZeroBox(&food);
    pxZeroBox(&poison);

    pxSetColor(&character.color, 0, 0, 1, 1);
    pxSetColor(&food.color, 0, 1, 0, 1);
    pxSetColor(&poison.color, 1, 0, 0, 1);

    pxSetBoxSize(&character, 0.7, 0.7);
    pxSetBoxSize(&food, 0.1, 0.1);
    pxSetBoxSize(&poison, 0.1, 0.1);

    pxSetColor(pxBackgroundColor(), 1, 1, 1, 1);
    pxRendererInit();
    pxTimerInit();

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
    }
}
