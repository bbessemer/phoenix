/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#pragma once

#include <stdio.h>

static char px_err[256];

static void pxFatal (const char *sysname, const char *msg, int line) {
    snprintf(px_err, 256, "Fatal error in system '%s': %s (line %d)",
        sysname, msg, line);
    puts(px_err);
    // TODO add messagebox
}
