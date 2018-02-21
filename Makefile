# vim: set noexpandtab:

###
# Copyright (C) 2018 Brent Bessemer.
# All rights reserved.
###

AR = ar -cr
CFLAGS = -O3 -ffast-math -fPIC
CC = clang -c -g
LLC = clang -c -g -O3
LD = clang

INCLUDEDIR = include
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lm -ldl

MACHINE= $(shell uname -s)
ifeq ($(MACHINE),Darwin)
    LIBS += -framework OpenGL
endif

all: target/libphoenix.a

target/libphoenix.a: target/audio.o target/bmpfont.o \
	target/box.o target/color.o	target/default_font.o target/gl_shaders.o \
	target/glad.o target/imgfile.o target/input.o target/renderer_gl.o \
	target/renderer.o target/texture_gl.o target/timer.o target/ttfont.o
	@echo "    AR  $@"
	@mkdir -p target
	@$(AR) $@ $^

dots:
	$(LD) -I $(INCLUDEDIR) test/dots.c -Ltarget -lphoenix $(LIBS) -o target/dots
	-target/dots

target/%.o: src/%.c $(INCLUDEDIR)/phoenix/*.h
	@echo "    CC  $<"
	@$(CC) $(CFLAGS) -I $(INCLUDEDIR) -o $@ $<

test/hitmarkers/%.o: test/hitmarkers/%.c
	@echo "    CC  $<"
	@$(CC) $(CFLAGS) -I $(INCLUDEDIR) -o $@ $<


target/%.llo: src/%.ll
	@echo "    LLC  $<"
	@$(LLC) -o $@ $<

clean:
	rm -v target/* test/**/*.o
