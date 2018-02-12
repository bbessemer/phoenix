/*
 * shaders.h
 *
 *  Created on: Jun 19, 2017
 *      Author: brentbessemer
 */

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <glad/glad.h>
#endif

const char pxShaderText_Box_V[] =
        "#version 150\n"
        "in vec2 pos;"
        "in vec2 tx_crd;"
        "out vec2 tx_crd_;"
        "uniform vec2 dims[4];"
        "uniform mat4 camera;"
        "void main ()"
        "{"
        "   mat2 dims_mat = mat2(dims[1].x, 0, 0, dims[1].y);"
        "   vec2 unrot_pos = dims_mat * (pos - dims[3]);"
        "   mat2 rotmat = mat2(dims[2], -dims[2].y, dims[2].x);"
        "   vec2 final_pos = (rotmat * unrot_pos) + (dims_mat * dims[3]);"
        "   tx_crd_ = tx_crd;"
        //"   gl_Position = camera * vec4(final_pos + 2*dims[0], 0, 1);"
        "   gl_Position = camera * vec4(final_pos + 2*dims[0], 0, 1);"
        "}";

const char pxShaderText_Box_FColor[] =
        "#version 150\n"
        "uniform vec4 color;"
        "out vec4 fragColor;"
        "void main ()"
        "{"
        "	fragColor = color;"
        //"   gl_FragColor = vec4(1, 1, 1, 1);"
        "}";

const char pxShaderText_Box_FTexture[] =
        "#version 150\n"
        "uniform sampler2D tex;"
        "in vec2 tx_crd_;"
        "uniform vec4 color;"
        "uniform bool texid;"
        "out vec4 fragColor;"
        "void main ()"
        "{"
        "   if (texid) {"
        "       fragColor = texture(tex, tx_crd_);"
        "   } else {"
        "       fragColor = color;"
        "   }"
        "}";

const GLfloat pxVertBuf_Box[] =
{
    -1., 1.,
     1., 1.,
    -1., -1.,
     1., -1.
};

const GLfloat pxTCBuf_Box[] =
{ 0., 0., 1., 0., 0., 1., 1., 1. };
