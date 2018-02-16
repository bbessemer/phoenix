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
pub struct Camera {
    dims: Box2D,
    mode: CameraMode,
    matrix: [[f32; 4]; 4],
}

impl Camera {
    pub fn set (&mut self, window: &Window, dims: Box2D, mode: CameraMode) {
        self.mode = match mode {
            CameraMode::NoChange => self.mode,
            _ => mode,
        };
        self.dims = dims;
        self.refresh(window)
    }

    pub fn refresh (&mut self, window: &Window) {
        let win_aspect = window.get_aspect();
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
    }
}
