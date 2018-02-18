/*
 * shaders.h
 *
 *  Created on: Jun 19, 2017
 *      Author: brentbessemer
 */

const char pxShaderText_Box_V[] =
#ifdef __EMSCRIPTEN__
    "#version 300 es\n"
#else
    "#version 150\n"
#endif
    "in vec2 pos;\n"
    "in vec2 tx_crd;\n"
    "out vec2 tx_crd_;\n"
    "uniform vec2 dims[3];\n"
    "uniform mat4 camera;\n"
    "void main ()\n"
    "{\n"
    "   mat2 dims_mat = mat2(dims[1].x, 0, 0, dims[1].y);\n"
    "   vec2 unrot_pos = dims_mat * pos;\n"
    "   mat2 rotmat = mat2(dims[2], -dims[2].y, dims[2].x);\n"
    "   vec2 final_pos = rotmat * unrot_pos;\n"
    "   tx_crd_ = tx_crd;\n"
    //"   gl_Position = camera * vec4(final_pos + 2*dims[0], 0, 1);"
    "   gl_Position = camera * vec4(final_pos + 2.0*dims[0], 0, 1);\n"
    "}";

const char pxShaderText_Box_FColor[] =
#ifdef __EMSCRIPTEN__
    "#version 300 es\n"
#else
    "#version 150\n"
#endif
    "uniform vec2 dims[5];\n"
    "out vec4 fragColor;\n"
    "void main ()\n"
    "{\n"
    "	fragColor = vec4(dims[3], dims[4]);\n"
    //"   gl_FragColor = vec4(1, 1, 1, 1);"
    "}";

const char pxShaderText_Box_FTexture[] =
#ifdef __EMSCRIPTEN__
    "#version 300 es\n"
#else
    "#version 150\n"
#endif
    "uniform sampler2D tex;\n"
    "in highp vec2 tx_crd_;\n"
    "uniform highp vec4 color;\n"
    "uniform bool texid;\n"
    "out highp vec4 fragColor;\n"
    "void main ()\n"
    "{\n"
    "   if (texid) {\n"
    "       fragColor = texture(tex, tx_crd_);\n"
    "   } else {\n"
    "       fragColor = color;\n"
    "   }\n"
    "}";

const float pxVertBuf_Box[] =
{
    -1., 1.,
     1., 1.,
    -1., -1.,
     1., -1.
};

const float pxTCBuf_Box[] =
{ 0., 0., 1., 0., 0., 1., 1., 1. };
