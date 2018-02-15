/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

extern crate sdl2_sys;
use std::slice;
use std::mem;
use sdl2_sys::{
    SDL_AudioSpec,
    SDL_OpenAudio,
    SDL_InitSubSystem,
    SDL_PauseAudio,
    SDL_INIT_AUDIO
};
use std::os::raw::c_void;

mod mixer;
mod ffi;

pub use mixer::Mixer;

pub const PX_SOUND_STOP: u32 = 0;
pub const PX_SOUND_LOOP: u32 = 1;

pub const PX_SOUND_PRESERVE: u32 = 255;

pub const PX_VOL_MIN: u32 = 0;
pub const PX_VOL_MAX: u32 = 1024;


#[repr(C, packed)]
pub struct SoundSource {
    pub format: u16,        /* proprietary format code */
    pub len: u32,           /* length of the audio in samples, little-endian */
    pub samplerate: u32,    /* samples per second, little-endian */
    pub stereo: u8,         /* 0 for mono, 1 for stereo (basically (channels - 1)) */
    samples: [u16; 0],
}

#[repr(C)]
pub struct Sound<'a> {
    pub src: &'a SoundSource,
    pub cur_sample: usize,
    pub pitch: f32,
    pub volume: i16,       /* on a scale of 0 to 1024 */
    pub cur_tick: u16,
    pub playing: bool,
    pub fate: u8,          /* see the defines above */
}

impl SoundSource {
    pub fn samples_as_slice (&self) -> &[u16] {
        let ptr = &self.samples as *const u16;
        unsafe { slice::from_raw_parts(ptr.offset(1), self.len as usize) }
    }
}

fn next_round (mut arg: u32) -> u32 {
    arg -= 1;
    arg |= arg >> 1;
    arg |= arg >> 2;
    arg |= arg >> 4;
    arg |= arg >> 8;
    arg |= arg >> 16;
    arg + 1
}

pub fn open<'a> (channels: u8, samplerate: i32, bufsecs: f32, format: u16,
    sounds: &[Sound<'a>]) -> Box<Mixer<'a>>
{
    let want = SDL_AudioSpec {
        freq: samplerate,
        format,
        channels,
        samples: next_round((bufsecs * samplerate as f32) as u32) as u16,
        callback: Some(Mixer::mix),
        userdata: unsafe { Box::into_raw(Box::new(Mixer::new(sounds))) as *mut c_void },
        size: 0,
        silence: 0,
        padding: 0,
    };
/*
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) puts(SDL_GetError());
    if (!SDL_OpenAudio(&want, &px_audio_spec)) SDL_PauseAudio(0);
    else printf("Audio failed to open: %s\n", SDL_GetError());
*/
    let spec: SDL_AudioSpec = mem::uninitialized();
    let mixer = Box::from_raw(spec.userdata as *mut Mixer<'a>);
    mixer.spec = spec;
    mixer
}
