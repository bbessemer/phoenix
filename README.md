# Phoenix Static Edition

This is a new version of my Phoenix game engine, written to exclusively use
static or application-allocated memory: that is, no `malloc` whatsoever.
Although this introduces a small amount of additional complexity for the
application developer, a significant gain in performance may be made by using a
contiguous block of RAM for each type of object, without the contingincies
necessary to allow that block to expand.

Obviously, it is not possible to avoid dynamic memory when dealing with large
multimedia objects such as textures and sounds. Static Edition does provide
functions for media loading which use `malloc`, but they have been segregated as
far as possible from the main core of the engine. If the developer wished to
replace them with his own utilities (for example, to support a new audio
format), he could do so easily.
