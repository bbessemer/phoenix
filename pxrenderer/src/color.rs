/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

pub struct Color {
    pub r: f32,
    pub g: f32,
    pub b: f32,
    pub a: f32,
}

impl Color {
    pub fn new (r: f32, g: f32, b: f32, a: f32) -> Self {
        Color { r, g, b, a }
    }

    pub fn from_u8 (r: u8, g: u8, b: u8, a: u8) -> Self {
        Color {
            r: r as f32 / 255.,
            g: g as f32 / 255.,
            b: b as f32 / 255.,
            a: a as f32 / 255.,
        }
    }
}
