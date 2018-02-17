/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

extern crate gl;
extern crate sdl2;
extern crate libc;

use self::gl::types::*; // This is bizarre behaviour.
use std::os::raw::c_void;
use super::super::*;

mod shaders;

struct GLUniforms {
    dims: GLint,
    camera: GLint,
    color: GLint,
    texid: GLint,
}

pub struct GLRenderer<'a> {
    pub window: &'a SDLWindow,
    pub camera: Camera,
    uniforms: GLUniforms,
    program: GLuint,
    background: Color,
    vao: GLuint,
    vbo_vert: GLuint,
    vbo_txc: GLuint,
}

const INDEX_VBO_VERT: GLuint = 0;
const INDEX_VBO_TXC: GLuint = 1;

fn cstr (string: &str) -> *const GLchar {
    string.as_ptr() as *const GLchar
}


impl GLUniforms {
    fn new (program: GLuint) -> Self { unsafe {
        Self {
            dims: gl::GetUniformLocation(program, cstr("dims\0")),
            camera: gl::GetUniformLocation(program, cstr("camera\0")),
            color: gl::GetUniformLocation(program, cstr("color\0")),
            texid: gl::GetUniformLocation(program, cstr("texid\0")),
        }
    }}
}

impl<'a> GLRenderer<'a> {
    unsafe fn init_static_vbo (index: GLuint, buf: &[GLfloat]) -> Result<GLuint, String>
    {
        let mut dest: GLuint = 0;
        gl::GenBuffers(1, &mut dest);
        if dest == 0 {
            Err(format!("Failed to generate buffer with index {}", index))
        } else {
            gl::BindBuffer(gl::ARRAY_BUFFER, dest);
            gl::BufferData(gl::ARRAY_BUFFER, 4 * buf.len() as isize,
                buf.as_ptr() as *const c_void, gl::STATIC_DRAW);
            Ok(dest)
        }
    }

    unsafe fn init_vaos (&mut self) -> Result<(), String> {
        gl::GenVertexArrays(1, &mut self.vao);
        if self.vao == 0 {
            Err("Failed to generate render array.".to_string())
        } else {
            gl::BindVertexArray(self.vao);

            let err: i32 = 0;

            gl::EnableVertexAttribArray(INDEX_VBO_VERT);
            match Self::init_static_vbo(INDEX_VBO_VERT, &shaders::VERT_BUF) {
                Err(msg) => return Err(msg),
                Ok(int) => self.vbo_vert = int,
            }
            gl::VertexAttribPointer(INDEX_VBO_VERT, 2, gl::FLOAT, gl::FALSE, 0,
                std::ptr::null());

            gl::EnableVertexAttribArray(INDEX_VBO_TXC);
            match Self::init_static_vbo(INDEX_VBO_TXC, &shaders::TXC_BUF) {
                Err(msg) => return Err(msg),
                Ok(int) => self.vbo_txc = int,
            }
            gl::VertexAttribPointer(INDEX_VBO_TXC, 2, gl::FLOAT, gl::FALSE, 0,
                std::ptr::null());

            Ok(())
        }
    }

    unsafe fn create_shader_prog (vsh: GLuint, fsh: GLuint) -> Result<GLuint, String> {
        let program = gl::CreateProgram();
        gl::UseProgram(program);
        gl::AttachShader(program, vsh);
        gl::AttachShader(program, fsh);
        gl::BindAttribLocation(program, INDEX_VBO_VERT, cstr("pos\0"));
        gl::BindAttribLocation(program, INDEX_VBO_VERT, cstr("tx_crd\0"));
        gl::LinkProgram(program);
        let mut err = gl::TRUE as i32;
        gl::GetProgramiv(program, gl::LINK_STATUS, &mut err as *mut i32);
        if err == gl::FALSE as i32
        {
            /*gl::GetProgramiv(program, gl::INFO_LOG_LENGTH, &err);
            log = malloc(err);
            gl::GetProgramInfoLog(program, err, std::ptr::null_mut(), log);
            asprintf(&last_err, "Program could not be linked\n\n%s", log);
            free(log);*/
            Err("Program could not be linked.".to_string())
        } else {
            Ok(program)
        }
    }

    unsafe fn init_shader_progs (&mut self) -> Result<(), String> {
        let vsh = gl::CreateShader(gl::VERTEX_SHADER);
        let vsh_src = cstr(shaders::VERTEX);
        gl::ShaderSource(vsh, 1, &vsh_src as *const *const i8, std::ptr::null());
        gl::CompileShader(vsh);
        let mut err = gl::TRUE as i32;
        gl::GetShaderiv(vsh, gl::COMPILE_STATUS, &mut err as *mut i32);
        if err == gl::FALSE as i32
        {
            /*gl::GetShaderiv(vsh, gl::INFO_LOG_LENGTH, &err);
            log = (char *)malloc(err);
            gl::GetShaderInfoLog(vsh, err, 0, log);
            asprintf(&last_err, "Vertex shader could not be compiled\n\n%s", log);
            free(log);
            return -1;*/
            return Err("Vertex shader could not be compiled.".to_string())
        }

        let fsh_tex = gl::CreateShader(gl::FRAGMENT_SHADER);
        let fsht_src = cstr(shaders::FRAGMENT);
        gl::ShaderSource(fsh_tex, 1, &fsht_src as *const *const i8, std::ptr::null());
        gl::CompileShader(fsh_tex);
        let mut err = gl::TRUE as i32;
        gl::GetShaderiv(fsh_tex, gl::COMPILE_STATUS, &mut err as *mut i32);
        if err == gl::FALSE as i32
        {
            /*gl::GetShaderiv(vsh, gl::INFO_LOG_LENGTH, &err);
            log = (char *)malloc(err);
            gl::GetShaderInfoLog(vsh, err, 0, log);
            asprintf(&last_err, "Vertex shader could not be compiled\n\n%s", log);
            free(log);
            return -1;*/
            return Err("Fragment shader could not be compiled.".to_string())
        }

        let prog_tex = Self::create_shader_prog(vsh, fsh_tex).unwrap();
        self.uniforms = GLUniforms::new(prog_tex);
        let fsh_unif_tex = gl::GetUniformLocation(prog_tex, cstr("tex\0"));
        gl::Uniform1i(fsh_unif_tex, 0);

        Ok(())
    }
}
