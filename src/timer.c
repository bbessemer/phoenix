/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#include <SDL2/SDL.h>
#include <phoenix/timer.h>
#include <phoenix/input.h>

static double perf_int;
static Uint64 last_step;
static float delta;
static unsigned long long frame_count;
static double game_time;
static unsigned int fps_interval;
static unsigned long long fps_last;
static double fps_last_time;
static void (*fpsCallback) (float);

float pxGetDelta () { return delta; }
Uint64 pxGetFrameCount () { return frame_count; }
double pxGetGameTime () { return game_time; }

void pxCountFPS (void (*callback) (float), unsigned int every) {
    fps_interval = every;
    fpsCallback = callback;
}

static void fpsStep ()
{
    float frames = (float)(frame_count - fps_last);
    float t = game_time - fps_last_time;
    fps_last = frame_count;
    fps_last_time = game_time;
    if (fpsCallback)
        fpsCallback(frames / t);
}

void pxTimerInit () {
    SDL_InitSubSystem(SDL_INIT_TIMER);
    perf_int = 1. / (double) SDL_GetPerformanceFrequency();
    frame_count = 0;
    game_time = 0;
    last_step = SDL_GetPerformanceCounter();
}

void pxTimerCycle (float *to_decrement, size_t n) {
    Uint64 now = SDL_GetPerformanceCounter();
    delta = (now - last_step) * perf_int;
    game_time += delta;
    last_step = now;
    frame_count++;

    if (fps_interval) if (frame_count % fps_interval == 0)
        fpsStep();

    for (size_t i = 0; i < n; i++) {
        to_decrement[i] -= delta;
    }
}

void pxDelay (float secs, void (*frame) ()) {
    for (; secs > 0 && !pxGetReqt(PX_REQT_EXIT); secs -= delta) frame();
}
