/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#include <phoenix/audio.h>

struct intfrac {
    size_t i;
    float f;    /* [0, 1) */
};

static struct intfrac getIntfrac (const float n) {
    return (struct intfrac){
        .i = (size_t) n,
        .f = n - (size_t) n
    };
}

static struct intfrac addIntfrac (const struct intfrac a,
    const struct intfrac b)
{
    return (struct intfrac){
        .i = a.i + b.i + (size_t)(a.f + b.f),
        .f = a.f + b.f - (size_t)(a.f + b.f)
    };
}

static unsigned short getNthSampleFromSwag (const px_sound_src_t *src,
    const struct intfrac n)
{
    const unsigned short *samples = (const unsigned short *)(src + 1);
    if (n.i < src->len - 1)
        return samples[n.i] * n.f + samples[n.i + 1] * (1.f - n.f);
    else return 0;
}

static unsigned short getNthSampleFromSound (const px_sound_t *sound,
    const int dest_rate, const int n)
{
    if (sound->src->format == 420)
        return getNthSampleFromSwag(sound->src,
            addIntfrac(*(struct intfrac*) &sound->cur_sample,
                getIntfrac(n * sound->src->samplerate / (float) dest_rate)));
    else return 0;
}

static unsigned short getNthSample (const px_sound_t *sounds,
    const size_t n_sounds, const int out_samplerate, const int n)
{
    unsigned short sample = 0;
    for (int i; i < n_sounds; i++)
        sample += getNthSampleFromSound(sounds + i, out_samplerate, n);
    return sample;
}
