/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

pub use super::*;

#[repr(C)]
#[derive(Clone, Copy)]
pub enum CameraMode {
    NoChange,
    Stretch,
    Overflow,
    Fit,
}

#[repr(C)]
#[derive(Clone, Copy)]
pub struct Camera {
    dims: Box2D,
    mode: CameraMode,
    pub matrix: [[f32; 4]; 4],
}

impl Camera {
    pub fn set (&mut self, aratio: f32, dims: Box2D, mode: CameraMode) -> Self {
        self.mode = match mode {
            CameraMode::NoChange => self.mode,
            _ => mode,
        };
        self.dims = dims;
        self.refresh(aratio);

        *self
    }

    pub fn refresh (&mut self, win_aspect: f32) -> Self {
        let adj_dims = match self.mode {
            CameraMode::Overflow => if win_aspect > self.dims.w / self.dims.h {
                Box2D { h: self.dims.w / win_aspect, ..self.dims }
            } else {
                Box2D { w: self.dims.h * win_aspect, ..self.dims }
            },
            CameraMode::Fit => if win_aspect < self.dims.w / self.dims.h {
                Box2D { h: self.dims.w / win_aspect, ..self.dims }
            } else {
                Box2D { w: self.dims.h * win_aspect, ..self.dims }
            },
            _ => self.dims
        };

        self.matrix[0][0] = 1. / adj_dims.w;
        self.matrix[1][1] = 1. / adj_dims.h;
        self.matrix[3][0] = -adj_dims.x * 0.5 / adj_dims.w;
        self.matrix[3][1] = -adj_dims.y * 0.5 / adj_dims.h;

        *self
    }
}

impl Default for Camera {
    fn default () -> Self {
        Camera {
            dims: Box2D {
                x: 0.,
                y: 0.,
                w: 2.,
                h: 2.,
                rotation: Rotation::new(0.),
            },
            mode: CameraMode::Fit,
            matrix: unsafe { std::mem::uninitialized() },
        }
    }
}
