#!/usr/bin/python

from build import ninja_syntax
import os
import glob
import platform
import subprocess

def libdir(path):
    return '-L' + path

def include(path):
    return '-I' + path

thisdir = os.path.dirname(os.path.realpath(__file__))

try:
    os.mkdir(os.path.join(thisdir, 'target'))
except OSError: # raised if directory already exists
    pass

cflags = ['-Ofast', '-g']
cppflags = ['-std=c++11']
includes = ['-I $phoenix/include']
libdirs = ['-L $phoenix/target']
libs = ['-lphoenix', '-lSDL2', '-lSDL2_image', '-lSDL2_ttf']

def find_sdl_win ():
    config_path = subprocess.check_output(['where.exe', 'sdl2-config'])
    return os.path.join(os.path.dirname(config_path.decode('utf-8')), '..')

exeext = ''

if platform.system() is 'Windows':
    sdl = find_sdl_win()
    cflags += ['-target', 'x86_64-pc-windows-gnu']
    includes += ['-I ' + os.path.join(sdl, 'include')]
    libdirs += ['-L ' + os.path.join(sdl, 'lib')]
    exeext = '.exe'

n = ninja_syntax.Writer(open('build.ninja', 'w'))

n.variable('phoenix', thisdir)
n.variable('builddir', '$phoenix/target')

n.variable('cc', 'clang')
n.variable('cpp', 'clang++')
n.variable('cflags', ' '.join(cflags))
n.variable('cppflags', '$cflags ' + ' '.join(cppflags))
n.variable('includes', ' '.join(includes))
n.variable('libdirs', ' '.join(libdirs))
n.variable('libs', ' '.join(libs))

n.newline()

n.rule('cc',
       command='$cc $cflags $target $includes -c $in -o $out',
       description='compile $in')

n.rule('cpp',
       command='$cpp $cppflags $includes -target $target -c $in -o $out',
       description='compile $in')

n.rule('link',
       command='$cpp $cppflags $in $libdirs $libs -o $out',
       description='link $out')

n.rule('archive',
       command='ar -cr $out $in',
       description='archive $out')

n.newline()

c_sources = [
    'audio',
    'bmpfont',
    'color',
    'default_font',
    'gl_shaders',
    'glad',
    'imgfile',
    'input',
    'renderer_gl',
    'renderer',
    'texture_gl',
    'timer',
    'ttfont'
]

n.newline()

def cc(src):
    return n.build(os.path.join('$builddir', src + '.o'), 'cc',
        os.path.join('src', src + '.c'))

objs = []
for src in c_sources:
    objs += cc(src)

n.newline()

dots_objs = []
dots_objs += n.build(os.path.join('$builddir', 'dots.o'), 'cc',
    os.path.join('test', 'dots.c'))

n.newline()

libphoenix = n.build(os.path.join('$builddir', 'libphoenix.a'), 'archive', objs)
n.build(os.path.join('$builddir', 'dots' + exeext), 'link', dots_objs, libphoenix)
