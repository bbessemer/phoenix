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
static float *ttl_array[] = {&ttl, NULL};
static int score;

void reset () {
    ttl = ROUND_TIME;
    srand(time(NULL));
    pxSetBoxPos(&food, rand() / (float) RAND_MAX * 1.5 - 1,
        rand() / (float) RAND_MAX * 1.5 - 1);
    pxSetBoxPos(&poison, rand() / (float) RAND_MAX * 1.5 - 1,
        rand() / (float) RAND_MAX * 1.5 - 1);
}

int main (void) {
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

    reset();
    while (!pxGetReqt(PX_REQT_EXIT)) {
        pxInputCycle();
        pxTimerCycle(ttl_array);

        pxGetMouse(&character.x, &character.y);

        if ((fabsf(food.x - character.x) < character.w / 2.)
            && (fabsf(food.y - character.y) < character.h / 2.))
        {
            reset();
            score++;
        }

        pxNewFrame();
        pxDrawBox(&food);
        pxDrawBox(&poison);
        pxDrawBox(&character);
    }
}
