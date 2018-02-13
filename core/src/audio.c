/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#include <SDL2/SDL.h>
#include <phoenix/audio.h>

static unsigned int internal_buffer[65536];
static SDL_AudioSpec px_audio_spec;
static px_sound_t *sounds;
static int n_sounds;

static void updateTick (size_t *j_in, int *tick, const unsigned int in_sr,
    const unsigned int out_sr, const int n_steps)
{
    const uint64_t ticks_per_step = (in_sr << 16) / out_sr;
    *tick += ticks_per_step * n_steps;
    *j_in += *tick >> 16;
    *tick &= 0xffff;
}

static void normalize (const unsigned int loudest, const int len) {
    const unsigned int norm = 0xffffffff / loudest;
    for (int i = 0; i < len; i++)
        internal_buffer[i] = (internal_buffer[i] * norm) >> 16;
}

static void premix (const int len) {
    memset(internal_buffer, 0, 65536 * sizeof(unsigned int));
    unsigned int loudest = 0;

    const unsigned int out_samplerate = px_audio_spec.freq;
    const char out_channels = px_audio_spec.channels;
    const unsigned int n_samples = len / out_channels;

    for (int i = 0; i < n_sounds; i++) if (sounds[i].playing) {
        const unsigned int in_samplerate = sounds[i].src->samplerate;
        const unsigned int in_channels = sounds[i].src->stereo + 1;
        unsigned short *samples = (unsigned short *)(sounds[i].src + 1);
        size_t j_in = sounds[i].cur_sample;
        int tick = sounds[i].cur_tick;

        if (in_channels == out_channels && in_samplerate == out_samplerate) {
            unsigned int idx_in = j_in * in_channels;
            for (int j = 0; j < n_samples; j++) {
                for (int k = 0; k < out_channels; k++, idx_in++) {
                    internal_buffer[j * out_channels + k] += samples[idx_in];
                    if (internal_buffer[j * out_channels + k] > loudest)
                        loudest = internal_buffer[j * out_channels + k];
                }
                if (++j_in >= sounds[i].src->len) {
                    if (sounds[i].fate == PX_SOUND_LOOP)
                        j_in -= sounds[i].src->len;
                    else {
                        sounds[i].playing = 0;
                        break;
                    }
                }
            }
        }

        else for (int j = 0; j < n_samples; j++) {
            int k_in = 0;
            for (int k = 0; k < out_channels; k++) {
                const int index = (in_channels * j_in) + k_in;
                internal_buffer[j * out_channels + k] += samples[index];
                if (internal_buffer[j * out_channels + k] > loudest)
                    loudest = internal_buffer[j * out_channels + k];
                if (++k_in >= in_channels) k_in = 0;
            }
            updateTick(&j_in, &tick, in_samplerate, out_samplerate, 1);
            if (j_in >= sounds[i].src->len) {
                if (sounds[i].fate == PX_SOUND_LOOP)
                    j_in -= sounds[i].src->len;
                else {
                    sounds[i].playing = 0;
                    break;
                }
            }
        }

        sounds[i].cur_sample = j_in;
        sounds[i].cur_tick = tick;
    }

    if (loudest > 0xffff) normalize(loudest, len);
}

void pxMixAudio (void *_unused, Uint8 *buffer, int buflen)
{
    const unsigned int *bufptr = internal_buffer;
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
            *(buffer_u8++) = *(bufptr++) >> 8;
        break;
    }
    case AUDIO_S8:
    {
        Sint8 *buffer_s8 = (Sint8 *)buffer;
        n_samples = buflen;
        premix(n_samples);
        for (int i = 0; i < n_samples; i++)
            *(buffer_s8++) = ((signed)*(bufptr++) - 0x8000) >> 8;
        break;
    }
    case AUDIO_U16:
    {
        Uint16 *buffer_u16 = (Uint16 *)buffer;
        n_samples = (buflen >> 1);
        premix(n_samples);
        for (int i = 0; i < n_samples; i++)
            *(buffer_u16++) = *(bufptr++);
        break;
    }
    case AUDIO_S16:
    {
        Sint16 *buffer_s16 = (Sint16 *)buffer;
        n_samples = (buflen >> 1);
        premix(n_samples);
        for (int i = 0; i < n_samples; i++)
            *(buffer_s16++) = (signed)*(bufptr++) - 0x8000;
        break;
    }
    case AUDIO_S32:
    {
        Sint32 *buffer_s32 = (Sint32 *)buffer;
        n_samples = (buflen >> 2);
        premix(n_samples);
        for (int i = 0; i < n_samples; i++)
            *(buffer_s32++) = ((signed)*(bufptr++) - 0x8000) << 16;
        break;
    }
    case AUDIO_F32:
    {
        float *buffer_f32 = (float *)buffer;
        n_samples = (buflen >> 2);
        premix(n_samples);
        for (int i = 0; i < n_samples; i++)
            *(buffer_f32++) = ((signed)*(bufptr++) - 0x8000) / 32768.0;
            //*(buffer_f32++) = sinf(i / 400.f);
        break;
    }}
}

static unsigned int nextRound (unsigned int arg) {
    arg--;
    arg |= (arg >> 1);
    arg |= (arg >> 2);
    arg |= (arg >> 4);
    arg |= (arg >> 8);
    arg |= (arg >> 16);
    return ++arg;
}

void pxOpenAudio (int channels, int samplerate, float bufsecs, Uint32 format,
    px_sound_t* sound_array, int n)
{
    SDL_AudioSpec want;

    memset(&want, 0, sizeof(SDL_AudioSpec));
    want.freq = samplerate;
#ifdef __APPLE__
    want.format = AUDIO_F32;
#else
    want.format = format;
#endif
    want.channels = channels;
    want.samples = nextRound((unsigned int)(bufsecs * samplerate));
    want.callback = pxMixAudio;

    sounds = sound_array;
    n_sounds = n;

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) puts(SDL_GetError());
    if (!SDL_OpenAudio(&want, &px_audio_spec)) SDL_PauseAudio(0);
    else printf("Audio failed to open: %s\n", SDL_GetError());
}
