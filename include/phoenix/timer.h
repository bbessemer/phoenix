/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#pragma once

#include <SDL2/SDL.h>

float pxGetDelta ();
Uint64 pxGetFrameCount ();
double pxGetGameTime ();

void pxTimerInit ();
void pxTimerCycle (float *, size_t);
