/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

pub mod geometry;
pub mod camera;
pub mod traits;
pub mod color;

pub use geometry::{Point, Rotation, Box2D};
pub use camera::{Camera, CameraMode};
pub use traits::{Window, Renderer2D};
pub use color::Color;

type Texture = u32;

pub struct Drawable {
    pub dims: Box2D,
    pub color: Color,
    pub texture: Texture,
}
