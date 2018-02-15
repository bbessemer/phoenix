/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

use std::slice;

mod mixer;
mod ffi;

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
