/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

use super::*;

pub struct Mixer<'a> {
    pub sounds: &'a [Sound<'a>],
    pub spec: SDL_AudioSpec,
}

impl<'a> Mixer<'a> {
    pub fn new (sounds: &'a [Sound<'a>]) -> Self {
        Self {
            sounds,
            spec: unsafe { mem::uninitialized() },
        }
    }

    fn mix_typed<T> (&mut self, buffer: &mut [T]) {

    }

    pub fn mix (&mut self, buffer: &mut [u8]) {
        unsafe { match self.spec.format {
            AUDIO_U8 => self.mix_typed(buffer),
            AUDIO_S8 => self.mix_typed(slice::from_raw_parts_mut(
                buffer.as_mut_ptr() as *mut i8, buffer.len())),
            AUDIO_U16 => self.mix_typed(slice::from_raw_parts_mut(
                buffer.as_mut_ptr() as *mut u16, buffer.len() / 2)),
            AUDIO_S16 => self.mix_typed(slice::from_raw_parts_mut(
                buffer.as_mut_ptr() as *mut i16, buffer.len() / 2)),
            AUDIO_S32 => self.mix_typed(slice::from_raw_parts_mut(
                buffer.as_mut_ptr() as *mut i32, buffer.len() / 4)),
            AUDIO_F32 => self.mix_typed(slice::from_raw_parts_mut(
                buffer.as_mut_ptr() as *mut f32, buffer.len() / 4)),
            _ => ()
        }}
    }
}

#[no_mangle]
pub unsafe extern "C" fn pxMixAudio<'a> (mixer: *mut c_void, buffer: *mut u8, buflen: i32) {
    Mixer::mix(&mut *(mixer as *mut Mixer<'a>),
        slice::from_raw_parts_mut(buffer, buflen as usize))
}
