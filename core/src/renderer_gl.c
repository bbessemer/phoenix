/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#define PX_COMPILE_GL
#define _GNU_SOURCE

#include <stdio.h>

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <phoenix/renderer.h>
#include <phoenix/input.h>
#include <phoenix/error.h>

SDL_Window *window;
SDL_GLContext gl_context;

static char *last_err;
static GLuint vao;
static GLuint vbo_vert;
static GLuint vbo_txc;

static const GLfloat null_rots[2] = {1., 0.};
static const GLfloat origin[2] = {0., 0.};

struct gl_uniforms
{
    GLint dims;
    GLint camera;
    GLint color;
    GLint texid;
};

static struct gl_uniforms color_unifs;
static struct gl_uniforms tex_unifs;

static GLuint prog_color;
static GLuint prog_tex;

extern const char pxShaderText_Box_V[];
extern const char pxShaderText_Box_FColor[];
extern const char pxShaderText_Box_FTexture[];
extern const GLfloat pxVertBuf_Box[];
extern const GLfloat pxTCBuf_Box[];

extern float camera_matrix[4][4];
extern unsigned int window_size_w, window_size_h;
extern px_color_t px_background_color;

#define index_vbo_vert  0
#define index_vbo_txc   1

static void swap_buffers (SDL_Window *window)
{
    glFinish();
    SDL_GL_SwapWindow(window);
}

static int
init_static_vbo (GLuint index, GLfloat* buf, size_t len, GLuint* dest)
{
    glGenBuffers(1, dest);
    if (!(*dest))
    {
        asprintf(&last_err, "Failed to generate buffer with index %u", index);
        return -1;
    }

    glBindBuffer(GL_ARRAY_BUFFER, *dest);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * len, buf, GL_STATIC_DRAW);
    return 0;
}

static int init_vaos (void)
{
    if (glGenVertexArrays == NULL)
        pxFatal("2D_GLRenderer", "glGenVertexArrays not found", __LINE__);
    glGenVertexArrays(1, &vao);
    if (!vao)
    {
        last_err = (char*) "Failed to generate render array.";
        return -1;
    }
    glBindVertexArray(vao);

    int err;

    glEnableVertexAttribArray(index_vbo_vert);
    err = init_static_vbo(index_vbo_vert, (GLfloat*) pxVertBuf_Box, 8,
            &vbo_vert);
    if (err < 0) return err;
    glVertexAttribPointer(index_vbo_vert, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(index_vbo_txc);
    err = init_static_vbo(index_vbo_txc, (GLfloat*) pxTCBuf_Box, 8, &vbo_txc);
    if (err < 0) return err;
    glVertexAttribPointer(index_vbo_txc, 2, GL_FLOAT, GL_FALSE, 0, 0);

    return 0;
}

static void init_uniforms (struct gl_uniforms *_this, GLuint program)
{
    _this->dims = glGetUniformLocation(program, "dims");
    _this->camera = glGetUniformLocation(program, "camera");
    _this->color = glGetUniformLocation(program, "color");
    _this->texid = glGetUniformLocation(program, "texid");
}

static GLuint create_shader_prog (GLuint vsh, GLuint fsh)
{
    GLuint program;
    GLint err;
    char *log;

    program = glCreateProgram();
    glUseProgram(program);
    glAttachShader(program, vsh);
    glAttachShader(program, fsh);
    glBindAttribLocation(program, index_vbo_vert, "pos");
    glBindAttribLocation(program, index_vbo_txc, "tx_crd");
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &err);
    if (err == GL_FALSE)
    {
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &err);
        log = malloc(err);
        glGetProgramInfoLog(program, err, 0, log);
        asprintf(&last_err, "Program could not be linked\n\n%s", log);
        free(log);
    }

    return program;
}

static int init_shader_progs (void)
{
    GLint err;
    char* log;

    GLuint vsh = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* vsh_src = pxShaderText_Box_V;
    glShaderSource(vsh, 1, &vsh_src, 0);
    glCompileShader(vsh);
    glGetShaderiv(vsh, GL_COMPILE_STATUS, &err);
    if (err == GL_FALSE)
    {
        glGetShaderiv(vsh, GL_INFO_LOG_LENGTH, &err);
        log = (char *)malloc(err);
        glGetShaderInfoLog(vsh, err, 0, log);
        asprintf(&last_err, "Vertex shader could not be compiled\n\n%s", log);
        free(log);
        return -1;
    }

    GLuint fsh_color = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar *fsh_src = pxShaderText_Box_FColor;
    glShaderSource(fsh_color, 1, &fsh_src, 0);
    glCompileShader(fsh_color);
    glGetShaderiv(fsh_color, GL_COMPILE_STATUS, &err);
    if (err == GL_FALSE)
    {
        glGetShaderiv(fsh_color, GL_INFO_LOG_LENGTH, &err);
        log = (char *)malloc(err);
        glGetShaderInfoLog(fsh_color, err, 0, log);
        asprintf(&last_err, "Fragment shader could not be compiled\n\n%s", log);
        free(log);
        return -2;
    }

    prog_color = create_shader_prog(vsh, fsh_color);
    init_uniforms(&color_unifs, prog_color);

    GLuint fsh_tex = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar *fsht_src = pxShaderText_Box_FTexture;
    glShaderSource(fsh_tex, 1, &fsht_src, 0);
    glCompileShader(fsh_tex);
    glGetShaderiv(fsh_tex, GL_COMPILE_STATUS, &err);
    if (err == GL_FALSE)
    {
        glGetShaderiv(fsh_tex, GL_INFO_LOG_LENGTH, &err);
        log = (char *)malloc(err);
        glGetShaderInfoLog(fsh_tex, err, 0, log);
        asprintf(&last_err, "Fragment shader could not be compiled\n\n%s", log);
        free(log);
        return -2;
    }

    prog_tex = create_shader_prog(vsh, fsh_tex);
    init_uniforms(&tex_unifs, prog_tex);
    GLint fsh_unif_tex = glGetUniformLocation(prog_tex, "tex");
    glUniform1i(fsh_unif_tex, 0);

    return 0;
}

void pxRendererInit_gl () {
    pxRendererInit();

    SDL_Window *win;
    SDL_GLContext gl_ctx;

    SDL_Init(SDL_INIT_VIDEO);

    /* Request opengl 3.2 context.
     * SDL doesn't have the ability to choose which profile at this time of writing,
     * but it should default to the core profile */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    win = SDL_CreateWindow("Phoenix Game Engine", SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, window_size_w, window_size_h,
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (!win) pxFatal("2D_GLRenderer", "Could not create window", __LINE__);

    gl_ctx = SDL_GL_CreateContext(win);
    if (!gl_ctx)
    {
        printf("%s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        pxFatal("2D_GLRenderer", "Could not create GL context", __LINE__);
    }

    if (!gladLoadGL())
    {
        SDL_GL_DeleteContext(gl_ctx);
        SDL_DestroyWindow(win);
        pxFatal("2D_GLRenderer", "Could not initialize OpenGL", __LINE__);
    }

    printf("Using OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    SDL_GL_SetSwapInterval(0);

    if (init_vaos() < 0)
    {
        SDL_GL_DeleteContext(gl_ctx);
        SDL_DestroyWindow(win);
        pxFatal("2D_GLRenderer", last_err, __LINE__);
    }

    if (init_shader_progs() < 0)
    {
        SDL_GL_DeleteContext(gl_ctx);
        SDL_DestroyWindow(win);
        pxFatal("2D_GLRenderer", last_err, __LINE__);
    }

    window = win;
    gl_context = gl_ctx;

    glUseProgram(prog_tex);
}

void pxNewFrame_gl () {
    swap_buffers(window);

    // Test for size change.
    if (pxGetReqt(PX_REQT_RESIZE))
    {
        glViewport(0, 0, window_size_w, window_size_h);
        pxRefreshCamera();
    }

    // Reset everything.
    glClearColor(px_background_color.r, px_background_color.g,
        px_background_color.b, px_background_color.a);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    // Camera matrix is easy; it's common to all boxes.
    glUniformMatrix4fv(tex_unifs.camera, 1, GL_FALSE, &camera_matrix[0][0]);
}

void pxDrawBoxes_gl (px_box_t *boxes, size_t n_boxes) {
    px_box_t * const end = boxes + n_boxes;
    for (px_box_t *box = boxes; box < end; box++)
        pxDrawBox_gl(box);
}

void pxDrawBox_gl (px_box_t *box) {
    /*if (box->color.a != 0)
    {
        glUseProgram(prog_color);
        glUniform2fv(color_unifs.dims, 4, (GLfloat *) box);
        glUniform4fv(color_unifs.color, 1, (GLfloat *) &box->color);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }*/

    if (box->texture || box->color.a != 0)
    {
        glUniform2fv(tex_unifs.dims, 4, (GLfloat *) box);
        glUniform4fv(tex_unifs.color, 1, (GLfloat *) &box->color);
        glUniform1ui(tex_unifs.texid, box->texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, box->texture);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}
