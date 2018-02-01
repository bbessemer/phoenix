/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#define PX_COMPILE_GL

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <glad/glad.h>
#endif

#include <SDL2/SDL.h>
#include <phoenix/texture.h>
#include <phoenix/error.h>

px_tex_t pxMakeTexture_gl (px_image_t *img) {
    return pxMakeTexture_Raw_gl(img->w, img->h, (px_icolor_t *)(img + 1));
}

px_tex_t pxMakeTexture_Raw_gl (unsigned int w, unsigned int h,
    px_icolor_t *pixels)
{
    GLuint tex_id = 0;
    if (glGenTextures == NULL)
        pxFatal("GLTextureLoader", "glGenTextures not found", __LINE__);
    glGenTextures(1, &tex_id);
    if (!tex_id) pxFatal("GLTextureLoader",
            "Could not generate texture object", __LINE__);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    return tex_id;
}

px_tex_t pxRemakeTexture_gl (px_tex_t old, px_image_t *new_img) {
    glDeleteTextures(1, &old);
    return pxMakeTexture_gl(new_img);
}
