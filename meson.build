project('phoenix', 'c')

sdl2 = dependency('sdl2')
sdl2_image = dependency('sdl2_image')
sdl2_ttf = dependency('sdl2_ttf')

incdir = include_directories('include')
subdir('src')

lib = static_library('phoenix', srcfiles, include_directories: incdir)
executable('dots', 'test/dots.c', include_directories: incdir, link_with: lib,
        dependencies: [sdl2, sdl2_image, sdl2_ttf])
executable('phoenix-bench', 'test/cpu_bench.c', include_directories: incdir, link_with: lib)
