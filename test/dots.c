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
