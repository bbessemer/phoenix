/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#include <phoenix/request.h>

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
