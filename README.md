# Phoenix (Static Edition)

Phoenix is a lightweight 2D game engine, written in C and based on
[SDL](http://libsdl.org) and OpenGL. Phoenix has made the unique design decision
to avoid dynamic memory allocation altogether: there are only a few calls to
`malloc` in the entire engine: a few to get OpenGL error strings, one to store
loaded image files, and one to store rendered text. This introduces a few
additional complexities for the end developer (more on this later), but
eliminates a number common game engine problems: the reallocation–cache
performance tradeoff, garbage collection, as well as the entire class of bugs
related to complex internal state. (Phoenix is not quite pure-functional—it is
very difficult to do that in C—but its design is certainly inspired by
functional programming concepts.) The lack of engine-provided dynamic allocation
(the developer may dynamically allocate anything he likes) means Phoenix may not
be suitable for all projects, but for the significant fraction that can be
written in an appropriate style, significant performance improvements may be
found by using it.

## A bit of justification

This is a very unusual project, for the reason that it requires the developer to
think in a rather different way about some things. Most engines must make a
choice about the allocation of memory for objects: either allocate each object
individually and dis-contiguously, and store pointers to each in a contiguous
list which is comparatively easy to reallocate (yet incurring a nearly 100%
cache miss rate due to the indirection); or store all objects (or all objects of
a class) in a single contiguous list which is extremely costly to grow, because
the full bulk of all the engine's objects must be moved if there is insufficient
free memory at the end of the list. A prior version of Phoenix created
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

## Performance

I haven't exactly done any formal benchmarking, but Phoenix reaches a hard GPU
bottleneck (at more than 9,000 FPS) on my NVIDIA GTX 1060 running an application
that draws less than 100 boxes at a resolution of only 1024×768. I dare you to
find a production engine that can do that.

This of course begs the question of who needs 9,000 FPS. No one does—but
performance is critical nowadays for battery reasons. If the game is capable of
running at 9,000 FPS, but has been vsynced to 60, then for 99.3% of the time
your CPU can turn itself off and go to sleep. Well, it's not quite this simple,
but that's the general idea. Even when running without vsync, profiling reveals
only 8% of the CPU time is actually spent in the engine; the rest is (mostly)
spent doing GL things.

## Licensing

Although I have published the source here, this is not quite an open-source
project. You may use it for your own personal purposes, but you may not publish,
for free or commercial purposes, anything written with it without my permission.
You also may not use any fragments of code contained here in another project.
