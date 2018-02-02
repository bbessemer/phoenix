/*
 * hitmarkers v3 (MLG Simulator 2006)
 *
 * Code copyright (C) 2017 Brent Bessemer.
 * All rights reserved.
 *
 * Original concept (as "MLG Simulator 2004" and "MLG Simulator 2005")
 * and assets copyright (C) 2014-2017 Seth Carter, if it's even possible to
 * copyright the concept of spraying a bunch of gaming-related logos and memes
 * onto a screen with 69 pixel-art hitmarkers and a black-and-white GIF of
 * Snoop Dogg dancing while shitty dubstep (is there any other kind?) plays
 * in the background.
 *
 * DISCLAIMER: ALL COPYRIGHTED IMAGES, MUSIC, SOUNDS, TRADEMARKS, LIKENESSES,
 * OR OTHER FORMS OF INTELLECTUAL PROPERTY DISPLAYED OR FEATURED HEREIN ARE
 * STRICTLY THE PROPERTY OF THEIR RESPECTIVE OWNERS. AUTHOR HEREBY CLAIMS FAIR
 * USE OF ANY COPYRIGHTED AND/OR TRADEMARKED ASSETS FEATURED HEREIN FOR THE
 * PURPOSE OF PARODY, AND OTHER STRICTLY NON-COMMERCIAL PURPOSES.
 */

#include <phoenix.h>

#include "../libswag/swag.h"
#include "memepaths.h"

#ifndef M_PI
#define M_PI    3.14159265359
#endif

// Let's make the code a little more MLG.
typedef char n00b;
typedef short derp;
typedef int mlg;
typedef float yolo;
typedef void swag;
typedef px_color hashtag;   // Perhaps a reference to HTML colour syntax
typedef struct px_image meme;
// Actually, all I've done is make it more Seth-like.

#define NOSCOPE 360
#define ak47 rand   // Accurate (no pun intended) simulation of firing an AK
#define reload() srand(SDL_GetPerformanceCounter())

// Forward declarations.
mlg load_swag (const n00b *path, derp **buffers, mlg fate);
mlg create_box_with_dims (yolo x, yolo y, yolo w, yolo h);

// Permanent entities
mlg bangarang;
mlg horns;
mlg hitmarkers[69];
mlg meme_sounds[8];
mlg snoop;

// Textures
mlg four[20]; // Snoop Dogg GIF (20 frames)
mlg hitmarker_tex;
mlg meme_texes[13];
mlg fashion_texes[44];

// Buffers: images
meme snoop_imgs[20];
meme meme_imgs[13];
meme fashion_imgs[44];
meme hitmarker_img;

// Buffers: sounds
derp *bang_bufs[2];
derp *horn_bufs[2];
derp *meme_snd_bufs[8][2];

// Fonts
TTF_Font *seguisym;

// Components
mlg cmp_hitmarker;
px_mask_t hm_mask;

#define MEMESIZE 0.3
#define BEAT (60. / 110.)
#define DEGREES (M_PI / 180.)

swag random_color (hashtag *color)
{
    reload();
    float h = NOSCOPE * (ak47() / (yolo)RAND_MAX);
    px_hsv_to_rgb(h, 0.9, 1.0, color);
}

mlg load_memes (mlg *loading)
{
    bangarang = load_swag("crap/bangarang.swag", bang_bufs, PX_SOUND_PRESERVE);
    horns = load_swag("crap/horns.swag", horn_bufs, PX_SOUND_PRESERVE);
    px_load_img("crap/hitmarker_image.png", &hitmarker_img);
    for (mlg i = 0; i < 8; i++)
        meme_sounds[i] = load_swag(meme_snd_paths[i], meme_snd_bufs[i],
                PX_SOUND_PRESERVE);
    for (mlg i = 0; i < 13; i++)
        px_load_img(meme_img_paths[i], &meme_imgs[i]);
    for (mlg i = 0; i < 20; i++)
    {
        char* snoop_path;
        asprintf(&snoop_path, "crap/snoops/frame_%03d.gif", i);
        px_load_img(snoop_path, &snoop_imgs[i]);
    }
    for (mlg i = 0; i < 44; i++)
    {
        px_color color;
        random_color(&color);
        px_render_text(fashion_statements[i], seguisym, &color,
                &fashion_imgs[i]);
    }

    *loading = 0;
    return 0;
}

yolo beat_time = 0;
yolo respawn_interval = 0;
swag respawn_random_hitmarker (swag);

swag spawn_hitmarkers (swag)
{
    cmp_hitmarker = px_create_component(0, 0);
    hm_mask = px_component_mask(cmp_hitmarker);
    reload();
    for (mlg i = 0; i < 69; i++)
    {
        yolo x = (NOSCOPE / 200.0) * (ak47() / (yolo)RAND_MAX - 0.5);
        const yolo y = (NOSCOPE / 200.0) * (ak47() / (yolo)RAND_MAX - 0.5);
        x *= px_get_window_aspect();
        mlg hm = create_box_with_dims(x, y, 0.2, 0.2);
        struct px2d_box *box = px_cmp_data(hm, px_cmp_box2d);

        px_add_component(hm, px_cmp_texture);
        *((mlg *)px_cmp_data(hm, px_cmp_texture)) = hitmarker_tex;

        const yolo rot = 1.571 * (ak47() / (yolo)RAND_MAX);
        px2d_rotation_init(&box->rot, rot);

        px_add_component(hm, cmp_hitmarker);
        hitmarkers[i] = hm;
        px_delay(0.04224306); // 1/69th of the length of the horn sound
    }
    respawn_interval = BEAT * 0.25;
}

swag spin_hitmarker (struct px2d_box **ptr)
{
    struct px2d_box *hm_box = *ptr;
    px2d_rotation_step(&hm_box->rot,
            -(NOSCOPE / 8.0) * DEGREES * px_get_delta());
}

swag spin_hitmarkers (swag)
{
    px_for_each(hm_mask, px_cmp_box2d, spin_hitmarker);

    beat_time += px_get_delta();
    if (respawn_interval && (beat_time > respawn_interval))
        respawn_random_hitmarker();
}

swag respawn_random_hitmarker (swag)
{
    reload();
    mlg hm = hitmarkers[ak47() % 69];
    struct px2d_box **ptr = px_cmp_data(hm, px_cmp_box2d);
    struct px2d_box *box = *ptr;
    //px2d_position *pos = px_cmp_data(hm, px_cmp_position2d);
    box->x = (NOSCOPE / 200.0) * (ak47() / (yolo)RAND_MAX - 0.5);
    box->x *= px_get_window_aspect();
    box->y = (NOSCOPE / 200.0) * (ak47() / (yolo)RAND_MAX - 0.5);
    const yolo rot = 1.571 * (ak47() / (yolo)RAND_MAX);
    px2d_rotation_init(&box->rot, rot);
    beat_time = 0;
}

swag replay (mlg sound, float vol)
{
    struct px_sound *snd_data;

    snd_data = px_cmp_data(sound, px_cmp_playing);
    snd_data->cur_sample = 0;
    snd_data->cur_tick = 0;
    snd_data->volume = 1024 * vol;
    px_add_component(sound, px_cmp_playing);
}

swag play_random_sound (float vol)
{
    reload();
    int i = 0;
    while (i++ < 8)
    {
        int rnd = ak47() % 8;
        int sound = meme_sounds[rnd];
        if (!px_has_component(sound, px_cmp_playing))
        {
            replay(sound, vol);
            break;
        }
    }
}

mlg meme_death (struct STT *stt, mlg meme, yolo ttl)
{
    struct px2d_box *boxdims = px_cmp_data(meme, px_cmp_box2d);
    const yolo aratio = boxdims->w / boxdims->h;
    const yolo scale = 1 + ttl / 0.069;
    if (scale < 0)
        return 0;
    else
    {
        boxdims->w = MEMESIZE * aratio * scale;
        boxdims->h = MEMESIZE * scale;
        return 1;
    }
}

swag show_random_meme (yolo x, yolo y, mlg spread)
{
    reload();
    if (spread)
    {
        x += 0.64 * (rand() / (yolo)RAND_MAX - 0.5);
        y += 0.64 * (rand() / (yolo)RAND_MAX - 0.5);
    }
    mlg thing = create_box_with_dims(x, y, 0.3, 0.3);
    px_add_component(thing, px_cmp_texture);
    mlg *tex = px_cmp_data(thing, px_cmp_texture);
    *tex = meme_texes[rand() % 13];
    px_set_ttl(thing, 0.420, meme_death);
}

swag loading_screen (swag)
{
    TTF_Init();
    TTF_Font* font;
    if ((font = TTF_OpenFont("seguisym.ttf", 64)));
    else if ((font = TTF_OpenFont("crap/seguisym.ttf", 64)));
    else px_fatal("MemeLoader", "Could not find font seguisym.ttf", __LINE__);
    seguisym = font;

    struct px_image illum_img;
    derp *illum_sndbuf[2];
    mlg illum_sound = load_swag("crap/illuminati.swag", illum_sndbuf,
            PX_SOUND_DESTROY);
    mlg illum_box = create_box_with_dims(0, 0, 0.8, 1.0);
    px_add_component(illum_box, px_cmp_texture);
    mlg *illum_tex = px_cmp_data(illum_box, px_cmp_texture);

    px_load_img("crap/illuminati.png", &illum_img);
    *illum_tex = px_create_texture(&illum_img);

    struct px_image textbox_img;
    px_color white = {1, 1, 1, 1};
    px_render_text("Brought to you by the Illuminati.", seguisym,
            &white, &textbox_img);
    yolo aratio = textbox_img.w / (yolo)textbox_img.h;
    mlg textbox = create_box_with_dims(0, -0.6, 0.8, 0.8 / aratio);
    px_add_component(textbox, px_cmp_texture);
    mlg *textbox_tex = px_cmp_data(textbox, px_cmp_texture);
    *textbox_tex = px_create_texture(&textbox_img);

    px_add_component(illum_sound, px_cmp_playing);
    mlg loading = 1;
    SDL_Thread *thr = SDL_CreateThread(load_memes, "MemeLoad", &loading);
    SDL_DetachThread(thr);

    while (loading)
        px_cycle();

    hitmarker_tex = px_create_texture(&hitmarker_img);
    for (mlg i = 0; i < 13; i++)
        meme_texes[i] = px_create_texture(&meme_imgs[i]);
    for (mlg i = 0; i < 20; i++)
        four[i] = px_create_texture(&snoop_imgs[i]);
    for (mlg i = 0; i < 44; i++)
        fashion_texes[i] = px_create_texture(&fashion_imgs[i]);

    while (px_has_component(illum_sound, px_cmp_playing))
        px_cycle();

    px_kill_entity(illum_box);
    free(illum_img.pixels);
    px_kill_entity(textbox);
    free(textbox_img.pixels);
}

swag insert_swag_here (float fps)
{
    printf("%f fps\n", fps);
}

mlg cur_snoop = 0;

mlg snoop_anim (struct STT *dead_snoop, mlg snoop, yolo ttl)
{
    mlg * const snooptex = px_cmp_data(snoop, px_cmp_texture);
    if (++cur_snoop >= 20) cur_snoop = 0;
    *snooptex = four[cur_snoop];
    dead_snoop->ttl = 0.1337;

    return 1;
}

swag onspray (swag *_unused)
{
    px2d_position click;

    px_get_mouse(&click);
    show_random_meme(click.x, click.y, 1);
    play_random_sound(0.3);
}

swag onsnipe (swag *_unused)
{
    px2d_position click;

    px_get_mouse(&click);
    show_random_meme(click.x, click.y, 0);
    play_random_sound(0.7);
}

yolo seizure_interval = BEAT;
yolo tts = 0;
yolo maxrot = NOSCOPE / 12.0 * DEGREES;
#define SB_INIT_ALPHA 0.75

mlg seizurebox_die (struct STT *stt, mlg box, yolo ttl)
{
    yolo deathtime = (seizure_interval < BEAT ? 0.1337 : 0.420);
    yolo a = SB_INIT_ALPHA * (1 + ttl / deathtime);
    if (a > 0)
    {
        ((px_color *)px_cmp_data(box, px_cmp_color))->a = a;
        return 1;
    }
    else return 0;
}

swag seizure (swag)
{
    reload();
    const yolo aratio = px_get_window_aspect();
    const yolo maxlen = 3 * sqrtf(1 + aratio * aratio);
    const yolo x = aratio * (ak47() / (yolo)RAND_MAX - 0.5);
    const yolo y = (ak47() / (yolo)RAND_MAX - 0.5);
    mlg seizurebox = create_box_with_dims(x, y, maxlen, 0.15);
    struct px2d_box **ptr = px_cmp_data(seizurebox, px_cmp_box2d);
    struct px2d_box *box = *ptr;
    const yolo rot = maxrot * 2 * (ak47() / (yolo)RAND_MAX - 0.5);
    px2d_rotation_init(&box->rot, rot);
    px_color *color = px_cmp_data(seizurebox, px_cmp_color);
    random_color(color);
    color->a = SB_INIT_ALPHA;
    px_add_component(seizurebox, px_cmp_color);
    px_set_ttl(seizurebox, 0, seizurebox_die);
}

yolo ttf = 0;

mlg fashionbox_die (swag) { return 0; }

swag fashion (swag)
{
    yolo maxrot = 30 * DEGREES;

    reload();
    const yolo w_aratio = px_get_window_aspect();
    const yolo x = 1.5 * w_aratio * (ak47() / (yolo)RAND_MAX - 0.5);
    const yolo y = 1.5 * (ak47() / (yolo)RAND_MAX - 0.5);
    mlg rnd = ak47() % 44;
    yolo aratio = fashion_imgs[rnd].w / (yolo)fashion_imgs[rnd].h;
    mlg fashionbox = create_box_with_dims(x, y, 0.15 * aratio, 0.15);
    struct px2d_box **ptr = px_cmp_data(fashionbox, px_cmp_box2d);
    struct px2d_box *box = *ptr;
    const yolo rot = maxrot * 2 * (ak47() / (yolo)RAND_MAX - 0.5);
    px2d_rotation_init(&box->rot, rot);
    mlg *tex = px_cmp_data(fashionbox, px_cmp_texture);
    *tex = fashion_texes[rnd];
    px_add_component(fashionbox, px_cmp_texture);
    px_set_ttl(fashionbox, 0.69, fashionbox_die);
}

swag seizure_cycle (swag)
{
    if (tts <= 0)
    {
        fashion();
        seizure();
        tts = seizure_interval;
    }
    else
        tts -= px_get_delta();
}

swag fashion_cycle (swag)
{
    if (ttf <= 0)
    {
        fashion();
        ttf = BEAT;
    }
    else ttf -= px_get_delta();
}

yolo ttm = 0;

swag meme_overload (swag)
{
    if (ttm <= 0)
    {
        reload();
        yolo x = (NOSCOPE / 200.0) * (ak47() / (yolo)RAND_MAX - 0.5);
        const yolo y = (NOSCOPE / 200.0) * (ak47() / (yolo)RAND_MAX - 0.5);
        x *= px_get_window_aspect();
        show_random_meme(x, y, 0);
        ttm = BEAT * 0.125;
    }
    else ttm -= px_get_delta();
}

static swag vocals (swag)
{
    maxrot = NOSCOPE / 12.0 * DEGREES;
    seizure_interval = BEAT;
}

static swag BASS (swag)
{
    maxrot = NOSCOPE / 6.0 * DEGREES;
    seizure_interval = 0.25 * BEAT;
}

mlg main (mlg argc, n00b **argv)
{
    px_init();
    px2d_use_renderer(PX_RENDERER_GL);
    px_set_window_title("MLG Simulator 2006");
    px2d_set_camera(0, 0, 2, 2, PX_CAM_FIT);

    px_fps_callback = insert_swag_here;
    px_count_fps_every(128);

    loading_screen();

    px_set_checkpoint();

    struct px_keybind *spray, *snipe;
    spray = px_keybind(PX_MOUSE_LEFT, KMOD_NONE);
    snipe = px_keybind(PX_MOUSE_RIGHT, KMOD_NONE);
    spray->repeat_interval = BEAT * 0.125;
    spray->onhold = onspray;
    snipe->onpress = onsnipe;

    while (1)
    {
        replay(horns, 0.5);

        px_add_system(spawn_hitmarkers, spin_hitmarkers, 0);
        px_systems_init();

        {
            const float aratio = snoop_imgs[0].w / snoop_imgs[0].h;
            snoop = create_box_with_dims(0.75, -0.5, aratio, 1);
            px_add_component(snoop, px_cmp_texture);
            mlg * const snooptex = px_cmp_data(snoop, px_cmp_texture);
            *snooptex = four[0];
            px_set_ttl(snoop, 0.1337, snoop_anim);
        }

        while (px_has_component(horns, px_cmp_playing)) px_cycle();

        px_add_system(0, seizure_cycle, 0);
        //px_add_system(0, fashion_cycle, 0);
        px_systems_init();

        replay(bangarang, 0.5);

        px_delay(48 * BEAT);
        BASS();
        px_delay(64 * BEAT);
        vocals();
        px_delay(32 * BEAT);
        BASS();
        px_delay(64 * BEAT);
        vocals();
        px_delay(32 * BEAT);
        seizure_interval = 2 * BEAT;
        px_delay(32 * BEAT);
        vocals();
        px_delay(32 * BEAT);
        BASS();
        int meme_spammer = px_add_system(0, meme_overload, 0);
        respawn_interval = BEAT / 69.;
        ((struct px_sound *)
                px_cmp_data(bangarang, px_cmp_playing))->volume = 676;
        px_delay(64 * BEAT);
        px_quit_system(meme_spammer);
        respawn_interval = 0.25 * BEAT;
        vocals();
        while (px_has_component(bangarang, px_cmp_playing))
            px_cycle();

        px_clear_entities();
    }
}

int create_box_with_dims (float x, float y, float w, float h)
{
    struct px2d_box *box_data, **ptr;

    int box = px_create_entity();
    px2d_make_box(box);
    px_add_component(box, px_cmp_box2d);
    ptr = px_cmp_data(box, px_cmp_box2d);
    box_data = *ptr;
    box_data->x = x;
    box_data->y = y;
    box_data->w = w;
    box_data->h = h;
    px2d_rotation_init(&box_data->rot, 0);
    return box;
}

mlg load_swag (const n00b* path, derp **buffers, mlg fate)
{
    struct swag_info bng_data;
    swag_fopen(path, &bng_data);
    buffers[0] = malloc(2 * bng_data.header.len);
    buffers[1] = malloc(2 * bng_data.header.len);
    size_t len = swag_read_pcm_s16(&bng_data, buffers, 0);
    int chn = (bng_data.header.stereo ? 2 : 1);
    int sr = bng_data.header.samplerate;
    swag_close(&bng_data);
    return px_create_sound(buffers, chn, sr, len, fate);
}
/*
#ifdef _WIN32

int CALLBACK WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
        LPSTR lpCmdLine, int nShowCmd)
{
    char* argv = 0;
    return main(0, &argv);
}

#endif
*/
