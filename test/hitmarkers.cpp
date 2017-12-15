/*
 * hitmarkers v3.2 (MLG Simulator 2006, Static Edition)
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

#include <phoenix/pse.h>

// MLG #defines
#define NOSCOPE 360.0
#define reload() srand(SDL_GetPerformanceCounter())

namespace mlg {

// Textures
px::Texture
four[20],   // Snoop Dogg GIF (20 frames)
hitmarker_tex,
meme_texes[13],
fashion_texes[44];

// Hitmarkers
px::StaticContainer<px::Box2D, 69> hitmarkers;

}
