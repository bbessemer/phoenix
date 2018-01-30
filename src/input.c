/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#include <phoenix/input.h>
#include <phoenix/renderer.h>
#include <phoenix/timer.h>

static unsigned int requests = 0;

unsigned int pxGetReqt (unsigned int flag) {
    return requests & flag;
}

void pxSetReqt (unsigned int flag) {
    requests |= flag;
}

void pxToggleReqt (unsigned int flag) {
    requests ^= flag;
}


/* EVENT POLLING */

static SDL_Event event;
float last_click_x, last_click_y;
extern unsigned int window_size_w, window_size_h;

void pxPollEvents () {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            pxSetReqt(PX_REQT_EXIT);
            break;
            case SDL_WINDOWEVENT:
            switch (event.window.event) {
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                window_size_w = event.window.data1;
                window_size_h = event.window.data2;
                pxSetReqt(PX_REQT_RESIZE);
                break;
                case SDL_WINDOWEVENT_CLOSE:
                pxSetReqt(PX_REQT_EXIT);
                default:
                break;
            }
            break;
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEBUTTONDOWN:
            last_click_x = event.button.x / (float) window_size_w;
            last_click_y = event.button.y / (float) window_size_h;
            pxScreentoWorld(&last_click_x, &last_click_y);
            /* no break */
            case SDL_KEYUP:
            case SDL_KEYDOWN:
            pxProcessKeyEvent(&event);
            break;
            default:
            continue;
        }
    }
}


/* MOUSE AND KEYBOARD HANDLING */

#define CTRL    1
#define SHIFT   2
#define ALT     3
#define CTRLSHF 4
#define CTRLALT 5
#define ALL     6
#define SHFALT  7

#define NUMKEYS 0x170
#define NUMMODS 8
#define NUMKEYBINDS (NUMMODS * NUMKEYS)

static px_keybind_t keybinds[NUMKEYBINDS];
static short keystates[NUMKEYS];

static short btn_encode (Uint32 btncode)
{
    if (btncode & PX_MOUSE)
        btncode = 64 + (btncode & 0x7fffffff);
    else if (btncode & 0x40000000)
        btncode = (0x80 - 0x39) + (btncode & 0x3fffffff);

    return btncode;
}

static short mod_encode (Uint16 modifiers)
{
    short modcode = 0;
    if (modifiers & KMOD_CMD) modcode += CTRL;
    if (modifiers & KMOD_SHIFT) modcode = modcode * 2 + SHIFT;
    if (modifiers & KMOD_ALT)
    {
        if (modcode < 4)
            modcode = modcode * 2 + ALT;
        else
            modcode = 6;
    }
    return modcode;
}

short pxButtonToIndex (Uint32 btncode, Uint16 modifiers) {
    return NUMMODS * btn_encode(btncode) + mod_encode(modifiers);
}

void pxProcessKeyEvent (SDL_Event *event)
{
    px_keybind_t *kb;
    SDL_KeyboardEvent *key;
    SDL_MouseButtonEvent *button;

    switch (event->type)
    {
    case SDL_KEYDOWN:
        key = &event->key;
        if (key->repeat) break;
        kb = keybinds + pxButtonToIndex(key->keysym.sym, key->keysym.mod);
        if (kb->onpress) kb->onpress(kb->userdata);
        pxSetKeyDown(btn_encode(key->keysym.sym),
                mod_encode(key->keysym.mod));
        break;
    case SDL_KEYUP:
        key = &event->key;
        kb = keybinds + pxButtonToIndex(key->keysym.sym, key->keysym.mod);
        if (kb->onrelease) kb->onpress(kb->userdata);
        pxSetKeyUp(btn_encode(key->keysym.sym));
        kb->tslp = 0;
        break;
    case SDL_MOUSEBUTTONDOWN:
        button = &event->button;
        kb = keybinds + pxButtonToIndex(PX_MOUSE | button->button, 0);
        if (kb->onpress) kb->onpress(kb->userdata);
        pxSetKeyDown(btn_encode(PX_MOUSE | button->button), 0);
        break;
    case SDL_MOUSEBUTTONUP:
        button = &event->button;
        kb = keybinds + pxButtonToIndex(PX_MOUSE | button->button, 0);
        if (kb->onrelease) kb->onrelease(kb->userdata);
        pxSetKeyUp(btn_encode(PX_MOUSE | button->button));
        kb->tslp = 0;
        break;
    default:
        break;
    }
}

void pxSetKeyDown (short button, short modifiers) {
    keystates[button] = modifiers + 1;
}

void pxSetKeyUp (short button) {
    keystates[button] = 0;
}

void pxInputCycle ()
{
    for (int i = 0; i < NUMKEYS; i++)
    {
        if (keystates[i])
        {
            px_keybind_t *kb;
            kb = keybinds + NUMMODS * i + (keystates[i] - 1);

            if (kb->tslp > kb->repeat_interval)
            {
                if (kb->onhold) kb->onhold(kb->userdata);
                kb->tslp = 0;
            }
            else kb->tslp += pxGetDelta();
        }
    }
}

px_keybind_t *pxGetKeybind (Uint32 button, Uint16 modifiers) {
    return keybinds + pxButtonToIndex(button, modifiers);
}
