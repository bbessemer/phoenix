/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

pub use super::*;

#[repr(C)]
pub enum CameraMode {
    NoChange,
    Stretch,
    Overflow,
    Fit,
}

#[repr(C)]
pub struct Camera {
    dims: Box2D,
    matrix: [[f32; 4]; 4],
}

impl Camera {
    
}
