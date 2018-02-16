/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#[repr(C)]
pub struct Point {
    x: f32,
    y: f32,
}

#[repr(C)]
pub struct Rotation {
    cos: f32,
    sin: f32,
}

impl Rotation {
    pub fn new (theta: f32) -> Self {
        Rotation { cos: theta.cos(), sin: theta.sin() }
    }

    pub fn step (&mut self, dt: f32) {
        *self = Rotation {
            cos: self.cos * (1. - dt*dt) - (self.sin * dt),
            sin: self.sin * (1. - dt*dt) + self.cos * dt,
        }
    }
}

#[repr(C)]
pub struct Box2D {
    x: f32,
    y: f32,
    w: f32,
    h: f32,
    rotation: Rotation,
}

impl Box2D {
    pub fn new (x: f32, y: f32, w: f32, h: f32, theta: f32) -> Self {
        Box2D {
            x, y, w, h,
            rotation: Rotation::new(theta),
        }
    }

    pub fn set_dims (&mut self, x: f32, y: f32, w: f32, h: f32) {
        self.x = x;
        self.y = y;
        self.w = w;
        self.h = h;
    }

    pub fn set_pos (&mut self, x: f32, y: f32) {
        self.x = x;
        self.y = y;
    }

    pub fn set_size (&mut self, w: f32, h: f32) {
        self.w = w;
        self.h = h;
    }
}
