/*
 * Eat Dots
 *
 * Concept and artwork copyright (C) 2014, Nathan Estock and Craig Rhodes.
 * Code copyright (C) 2018, Brent Bessemer.
 * All rights reserved.
 */

 #define PX_USE_GL

 #include <phoenix/phoenix.h>

 int main (void) {
     px_box_t character, food, poison;

     pxZeroBox(&character);
     pxZeroBox(&food);
     pxZeroBox(&poison);

     pxSetColor(&character.color, 0, 0, 1, 1);
     pxSetColor(&food.color, 0, 1, 0, 1);
     pxSetColor(&poison.color, 1, 0, 0, 1);

     character.texture = 0;
     food.texture = 0;
     poison.texture = 0;

     pxSetBoxDims(&character, 0, 0, 0.7, 0.7);
     pxSetBoxDims(&food, -0.5, -0.5, 0.1, 0.1);
     pxSetBoxDims(&poison, 0.5, 0.5, 0.1, 0.1);

     pxSetColor(pxBackgroundColor(), 1, 1, 1, 1);
     pxRendererInit();
     pxTimerInit();
     while (!pxGetReqt(PX_REQT_EXIT)) {
         pxInputCycle();
         pxTimerCycle(NULL);
         pxNewFrame();
         pxDrawBox(&character);
         pxDrawBox(&food);
         pxDrawBox(&poison);
     }
 }
