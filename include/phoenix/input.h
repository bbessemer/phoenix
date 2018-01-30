/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#pragma once

#include <SDL2/SDL.h>

#define PX_REQT_EXIT    0x01
#define PX_REQT_RESIZE  0x02

unsigned int pxGetReqt (unsigned int flag);
void pxSetReqt (unsigned int flag);
void pxToggleReqt (unsigned int flag);

void pxPollEvents ();

#define PX_MOUSE        0x80000000
#define PX_MOUSE_LEFT   (PX_MOUSE | SDL_BUTTON_LEFT)
#define PX_MOUSE_RIGHT  (PX_MOUSE | SDL_BUTTON_RIGHT)
#define PX_MOUSE_MID    (PX_MOUSE | SDL_BUTTON_MIDDLE)

#ifdef __APPLE__
#define KMOD_LCMD   KMOD_LGUI
#define KMOD_RCMD   KMOD_RGUI
#define KMOD_CMD    KMOD_GUI
#else
#define KMOD_LCMD   KMOD_LCTRL
#define KMOD_RCMD   KMOD_RCTRL
#define KMOD_CMD    KMOD_CTRL
#endif


typedef struct px_keybind
{
    void (*onpress) (void *);
    void (*onrelease) (void *);
    void (*onhold) (void *);
    void *userdata;
    Uint32 button;
    float repeat_interval;
    float tslp;              /* time since last press */
    Uint16 modifiers;
} px_keybind_t;

short pxButtonToIndex (Uint32 button, Uint16 modifiers);
void pxProcessKeyEvent (SDL_Event *event);
void pxSetKeyDown (short button, short modifiers);
void pxSetKeyUp (short button);
px_keybind_t *pxGetKeybind (Uint32 button, Uint16 modifiers);
