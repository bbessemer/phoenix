/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

#define PX_SOUND_DESTROY    0
#define PX_SOUND_LOOP       1

#define PX_SOUND_PRESERVE   255

#define PX_VOL_MIN     0
#define PX_VOL_MAX  1024
#define PX_VOL(n) ((short)(1024 * (n)))

/*
 * Astute readers may notice something about this sound source format. Well,
 * first, you'll notice that there's no pointer to the actual samples of the
 * sound in the structure. That's because the sound samples always follow
 * directly after the end of the structure. Second, if you're one of maybe two
 * people in the world, you'd notice that the structure is identical to the file
 * header of a SWAG file. Well, so is the sample data format. Which means that
 * you can simply mmap() a .swag file and it will be a valid px_sound_src. I
 * don't know who would want to do this besides me (and then only in one
 * application), but it can be done. Why invent a new format for anything?
 */

#pragma push(pack, 1)
typedef struct px_sound_src {
    uint16_t format;        /* proprietary format code */
    uint32_t len;           /* length of the audio in samples, little-endian */
    uint32_t samplerate;    /* samples per second, little-endian */
    uint8_t stereo;         /* 0 for mono, 1 for stereo (basically (channels - 1)) */
} __attribute__((packed)) px_sound_src_t;
#pragma pop(pack)

typedef struct px_sound {
    px_sound_src_t *src;
    size_t cur_sample;
    float pitch;
    short volume;       /* on a scale of 0 to 1024 */
    unsigned short cur_tick;
    unsigned char fate; /* see the defines above */
} px_sound_t;
