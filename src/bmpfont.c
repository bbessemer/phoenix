/*
 * Copyright (C) 2017-2018 Brent Bessemer.
 * All rights reserved.
 */

#include <string.h>
#include <phoenix/font.h>

void pxMakeBitmapFont (px_bmpfont_t *font, px_image_t *image,
    const char *string, int char_h)
{
    font->char_w = image->w;
    font->char_h = char_h;
    size_t len = strlen(string) + 1;
    strncpy(font->chars, string, 256);
    if (len > 256) {
        font->chars[255] = 0;
        len = 256;
    }
    memcpy(font + 1, image + 1, char_h * image->w * len * sizeof(px_icolor_t));
}

extern struct {} px_dflt_font_img;

void pxMakeDefaultFont (px_bmpfont_t *font) {
    pxMakeBitmapFont(font, (px_image_t *) &px_dflt_font_img,
        "ABCDEFGHIJKLMNOPQRSTUVWZYZabcdefghijklmnopqrstuvwzyz0123456789-", 5);
}

void pxDrawBitmapChar (px_image_t *out, const px_bmpfont_t *font,
    const size_t i, char c)
{
    px_icolor_t *img_data = (px_icolor_t *)(out + 1);
    px_icolor_t *font_data = (px_icolor_t *)(font + 1);
    int char_i = 0;
    for (int j = 0; 1; j++) if (font->chars[j] == c || !font->chars[j]) {
        char_i = j;
        break;
    }
    for (int j = 0; j < font->char_h; j++) {
        memcpy(&img_data[j * out->w + i * font->char_w],
            &font_data[font->char_w * (char_i * font->char_h + j)],
            font->char_w * sizeof(px_icolor_t));
    }
}

void pxRenderTextBitmap (px_image_t *out, const px_bmpfont_t *font,
    const char *string)
{
    memset(out + 1, 0, out->h * out->w * sizeof(px_icolor_t));
    for (int i = 0; *string; string++, i++)
        pxDrawBitmapChar(out, font, i, *string);
}
