/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

extern crate sdl2;
extern crate gl;

use super::super::traits::{Window, GLWindow};
use std::os::raw::c_void;

pub struct SDLWindow {
    video_sys: sdl2::VideoSubsystem,
    win: sdl2::video::Window,
}

impl SDLWindow {
    pub fn new () -> Self {
        let video_sys = sdl2::init().unwrap().video().unwrap();
        let win = video_sys.window("Phoenix Game Engine",
             800, 600)
            .position_centered()
            .resizable()
            .opengl()
            .build().unwrap();
        SDLWindow { video_sys, win }
    }
}

impl Window for SDLWindow {
    fn get_w (&self) -> u32 {
        self.win.size().0
    }

    fn get_h (&self) -> u32 {
        self.win.size().1
    }
}

impl GLWindow for SDLWindow {
    fn get_context (&mut self) {
        use self::sdl2::video::GLProfile;

        let gl_attr = self.video_sys.gl_attr();
        if !cfg!(target_os = "emscripten") {
            gl_attr.set_context_flags().forward_compatible().set();
            gl_attr.set_context_profile(GLProfile::Core);
            gl_attr.set_context_version(3, 3);
        }
        self.win.gl_create_context().unwrap();
        gl::load_with(|s| self.video_sys.gl_get_proc_address(s) as *const c_void);
    }

    fn swap (&mut self) {
        self.win.gl_swap_window();
    }

    fn set_vsync (&mut self, vsync: bool) {
        self.video_sys.gl_set_swap_interval(vsync as i32);
    }
}
