# Phoenix Static Edition

This is a new version of my Phoenix game engine, written to exclusively use
static or application-allocated memory: that is, no `malloc` whatsoever.
Although this introduces a small amount of additional complexity for the
application developer, a significant gain in performance may be made by using a
contiguous block of RAM for each type of object, without the contingincies
necessary to allow that block to expand.
