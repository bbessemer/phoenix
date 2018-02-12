# vim: set noexpandtab:

###
# Copyright (C) 2018 Brent Bessemer.
# All rights reserved.
###

AR = ar -cr
CFLAGS =  -fPIC
CC = gcc -c -g
LLC = clang -c -g -O3
LD = cc

INCLUDEDIR = include
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lm -ldl

MACHINE= $(shell uname -s)
ifeq ($(MACHINE),Darwin)
    LIBS += -framework OpenGL
endif

all: target/libphoenix.a

target/libphoenix.a: src/audio.o src/bmpfont.o src/box.llo src/box.o src/color.o \
	src/default_font.o src/gl_shaders.o src/glad.o src/imgfile.o src/input.o \
	src/renderer_gl.o src/renderer.o src/texture_gl.o src/timer.o src/ttfont.o
	@echo "    AR  $@"
	@mkdir -p target
	@$(AR) $@ $^

hitmarkers: target/hitmarkers target/libphoenix.a
	-./target/hitmarkers
	rm target/hitmarkers

target/hitmarkers: test/hitmarkers/hitmarkers.o \
	test/hitmarkers/hitmarker_image.o
	@echo "    LD  $@"
	$(LD) $^ -Ltarget -lphoenix $(LIBS) -o $@

%.o: %.c $(INCLUDEDIR)/phoenix/*.h
	@echo "    CC  $<"
	@$(CC) $(CFLAGS) -I $(INCLUDEDIR) -o $@ $<

%.llo: %.ll
	@echo "    LLC  $<"
	@$(LLC) -o $@ $<

clean:
	rm -v src/*.o src/*.llo target/* test/**/*.o
