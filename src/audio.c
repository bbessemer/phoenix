/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#include <phoenix/audio.h>

void pxGetSoundSamples_u16 (unsigned short *dest, px_sound_t *sound,
    int out_samplerate, int out_ch, int samples)
{
    unsigned int in_sr = sound->src->samplerate;
    int in_ch = sound->src->stereo + 1;
    unsigned short *in_samp_a = (short *)(sound + 1)
        + (sound->cur_sample * in_ch);
    unsigned short *in_samp_b = in_samp_a + in_ch;

    int ticks = (in_sr << 16) / out_samplerate;
    ticks *= sound->pitch;
    int spf = (ticks & 0xffff0000) >> 16;   // samples per frame
    int cur_tick = sound->cur_tick;

    for (int i = 0; i < samples; i++) {
        unsigned int a = *in_samp_a;
        unsigned int b = *in_samp_b;
        a *= (unsigned int)(0x10000 - cur_tick);
        a += b * (unsigned int)(cur_tick);
        a >>= 16;
        a = (a * (int)sound->volume) >> 10;
    }
}
