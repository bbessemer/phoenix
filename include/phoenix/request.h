/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#pragma once

#define PX_REQT_EXIT    0x01
#define PX_REQT_RESIZE  0x02

unsigned int pxGetReqt (unsigned int flag);
void pxSetReqt (unsigned int flag);
void pxToggleReqt (unsigned int flag);
