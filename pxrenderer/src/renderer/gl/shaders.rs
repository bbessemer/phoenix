/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

pub const VERTEX: &'static str =
    "#version 150
    in vec2 pos;
    in vec2 tx_crd;
    out vec2 tx_crd_;
    uniform vec2 dims[3];
    uniform mat4 camera;
    void main ()
    {
        mat2 dims_mat = mat2(dims[1].x, 0, 0, dims[1].y);
        vec2 unrot_pos = dims_mat * pos;
        mat2 rotmat = mat2(dims[2], -dims[2].y, dims[2].x);
        vec2 final_pos = rotmat * unrot_pos;
        tx_crd_ = tx_crd;
        gl_Position = camera * vec4(final_pos + 2.0*dims[0], 0, 1);
    }\0";

pub const FRAGMENT: &'static str =
    "#version 150
    uniform sampler2D tex;
    in highp vec2 tx_crd_;
    uniform highp vec4 color;
    uniform bool texid;
    out highp vec4 fragColor;
    void main ()
    {
       if (texid) {
           fragColor = texture(tex, tx_crd_);
       } else {
           fragColor = color;
       }
   }\0";

pub const VERT_BUF: [f32; 8] = [
    -1., 1.,
     1., 1.,
    -1., -1.,
     1., -1.
];

pub const TXC_BUF: [f32; 8] = [0., 0., 1., 0., 0., 1., 1., 1.];
