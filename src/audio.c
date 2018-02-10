/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#include <SDL2/SDL.h>
#include <phoenix/audio.h>

static int internal_buffer[65536];
static SDL_AudioSpec px_audio_spec;

static void premix (int len) {}

void pxMixAudio (void *_unused, Uint8 *buffer, int buflen)
{
    const int *bufptr = internal_buffer;
    int n_samples;
    int channels = px_audio_spec.channels;

    memset(buffer, 0, buflen);

    switch (px_audio_spec.format)
    {
    case AUDIO_U8:
    {
        Uint8 *buffer_u8 = buffer;
        n_samples = buflen;
        premix(n_samples);
        for (int i = 0; i < n_samples; i++)
            *(buffer_u8++) = ((unsigned)*(bufptr++) + 0x8000) >> 8;
        break;
    }
    case AUDIO_S8:
    {
        Sint8 *buffer_s8 = (Sint8 *)buffer;
        n_samples = buflen;
        premix(n_samples);
        for (int i = 0; i < n_samples; i++)
            *(buffer_s8++) = *(bufptr++) >> 8;
        break;
    }
    case AUDIO_U16:
    {
        Uint16 *buffer_u16 = (Uint16 *)buffer;
        n_samples = (buflen >> 1);
        premix(n_samples);
        for (int i = 0; i < n_samples; i++)
            *(buffer_u16++) = (unsigned)*(bufptr++) + 0x8000;
        break;
    }
    case AUDIO_S16:
    {
        Sint16 *buffer_s16 = (Sint16 *)buffer;
        n_samples = (buflen >> 1);
        premix(n_samples);
        for (int i = 0; i < n_samples; i++)
            *(buffer_s16++) = *(bufptr++);
        break;
    }
    case AUDIO_S32:
    {
        Sint32 *buffer_s32 = (Sint32 *)buffer;
        n_samples = (buflen >> 2);
        premix(n_samples);
        for (int i = 0; i < n_samples; i++)
            *(buffer_s32++) = *(bufptr++) << 16;
        break;
    }
    case AUDIO_F32:
    {
        float *buffer_f32 = (float *)buffer;
        n_samples = (buflen >> 2);
        premix(n_samples);
        for (int i = 0; i < n_samples; i++)
            *(buffer_f32++) = *(bufptr++) / 32768.0;
        break;
    }}
}

void pxOpenAudio (int channels, int samplerate, float bufsecs, Uint32 format) {
    SDL_AudioSpec want;

    memset(&want, 0, sizeof(SDL_AudioSpec));
    want.freq = samplerate;
    want.format = format;
    want.channels = channels;
    want.samples = px_next_round((unsigned int)(bufsecs * samplerate));
    want.callback = pxMixAudio;

    if (!SDL_OpenAudio(&want, &px_audio_spec)) SDL_PauseAudio(0);
}
