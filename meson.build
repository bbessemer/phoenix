project('phoenix', 'c')

sdl2 = dependency('SDL2')
sdl2_image = dependency('SDL2_image')
sdl2_ttf = dependency('SDL2_ttf')
libm = meson.get_compiler('c').find_library('m')
libdl = meson.get_compiler('c').find_library('dl')

incdir = include_directories('include')
subdir('src')

lib = static_library('phoenix', srcfiles, include_directories: incdir)
executable('dots', 'test/dots.c', include_directories: incdir, link_with: lib,
	dependencies: [sdl2_image, sdl2_ttf, sdl2, libm, libdl])
# executable('phoenix-bench', 'test/cpu_bench.c', include_directories: incdir, link_with: lib)
