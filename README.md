# Phoenix Static Edition

This is a new version of my Phoenix game engine, written to exclusively use
static or application-allocated memory: that is, no `malloc` whatsoever.
Although this introduces a small amount of additional complexity for the
application developer, a significant gain in performance may be made by using a
contiguous block of RAM for each type of object, without the contingencies
necessary to allow that block to expand.

Obviously, it is not possible to avoid dynamic memory when dealing with large
multimedia objects such as textures and sounds. Static Edition does provide
functions for media loading which use `malloc`, but they have been segregated as
far as possible from the main core of the engine. If the developer wished to
replace them with his own utilities (for example, to support a new audio
format), he could do so easily.

In tests I have achieved a framerate gain of roughly 50–100%, depending on
hardware, over the prior version of Phoenix.

## What the hell?

This is a very unusual project, for the reason that it requires the developer to
think in a rather different way about some things. Most engines must make a
choice about the allocation of memory for objects: either allocate each object
individually and dis-contiguously, and store pointers to each in a contiguous
list which is comparatively easy to reallocate (yet incurring a nearly 100%
cache miss rate due to the indirection); or store all objects (or all objects of
a class) in a single contiguous list which is extremely costly to grow, because
the full bulk of all the engine's objects must be moved if there is insufficient
free memory at the end of the list. The prior version of Phoenix created
a novel solution to this problem: objects were allocated in large, fixed-sized
blocks which were joined in a linked list. However, the code to implement these
'memory pages' proved to be extremely complex and prone to bugs.

Phoenix Static Edition 'solves' this problem by not attempting to solve it. All
objects must be explicitly allocated by the application. This does provide an
additional burden for the application developer, yet the application developer
is often far better equipped to handle it than is the engine. Many games have
either a fixed number of objects or a number of objects which can be expected
to remain under a reasonable fixed maximum. In this case the memory for those
objects can be hard-coded into the application. A good example of this is the
source code for the engine's test application (abbreviated somewhat for
readability):

```c
static px_box_t hitmarkers[69];
static px_box_t snoop;
static px_box_t memes[MEME_MAX];
static px_box_t strobes[STROBE_MAX];
static px_box_t fashions[FASHION_MAX];
static float ttls[N_TTLS];

// Game logic ...

void tick () {
    pxInputCycle();
    pxTimerCycle(ttls, N_TTLS);

    // Game Logic ...

    pxNewFrame();
    pxDrawBoxes(hitmarkers, 69);
    pxDrawBox(&snoop);
    pxDrawBoxes(strobes, STROBE_MAX);
    pxDrawBoxes(fashions, FASHION_MAX);
    pxDrawBoxes(memes, MEME_MAX);
}

int main (int argc, char **argv) {
    srand(time(NULL));
    pxRendererInit();
    pxTimerInit();
    pxTTFInit("crap/seguisym.ttf");
    PrepareTextures();
    pxCountFPS(PrintFPS, 1000);

    // Game logic ...

    while (!pxGetReqt(PX_REQT_EXIT)) tick();
    return 0;
}
```

If an unknown and potentially unlimited number of objects must be used, the
developer can dynamically allocate a list as easily as the engine could;
however, it does not need to be done every time. The other interesting part
of the Hitmarkers source is the spawning functions, which do not so much spawn
a new object as re-use a dead one, for example:

```c
void ShowRandomMeme (float x, float y) {
    for (int i = 0; i < MEME_MAX; i++) if (memes[i].texture == 0) {
        pxSetBoxDims(memes + i, x, y, 0.3, 0.3);
        memes[i].texture = meme_texes[rand() % 13];
        ttls[TTL_MEME_START + i] = 0.420;
        return;
    }
}
```
