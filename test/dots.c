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
     pxSetColor(pxBackgroundColor(), 1, 1, 1, 1);
     pxRendererInit();
     pxTimerInit();
     while (!pxGetReqt(PX_REQT_EXIT)) {
         pxInputCycle();
         pxTimerCycle(NULL);
         pxNewFrame();
     }
 }
