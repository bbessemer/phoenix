/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

use super::*;

pub trait Window {
    fn get_w (&self) -> u32;
    fn get_h (&self) -> u32;
    fn get_aspect (&self) -> f32 {
        self.get_w() as f32 / self.get_h() as f32
    }
}

pub trait Renderer2D {
    fn draw (&mut self, objects: &[Drawable]);
    fn new_frame (&mut self);
    fn set_background (&mut self, color: Color);
}
