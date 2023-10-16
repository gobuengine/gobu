/**********************************************************************************
 * gobu_gobu_embed.h                                                              *
 * ********************************************************************************
 * GOBU ENGINE                                                                    *
 * https://gobuengine.org                                                         *
 * ********************************************************************************
 * Copyright (c) 2023 Jhonson Ozuna Mejia                                         *
 *                                                                                *
 * Permission is hereby granted, free of charge, to any person obtaining a copy   *
 * of this software and associated documentation files (the "Software"), to deal  *
 * in the Software without restriction, including without limitation the rights   *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      *
 * copies of the Software, and to permit persons to whom the Software is          *
 * furnished to do so, subject to the following conditions:                       *
 *                                                                                *
 * The above copyright notice and this permission notice shall be included in all *
 * copies or substantial portions of the Software.                                *
 *                                                                                *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  *
 * SOFTWARE.                                                                      *
 **********************************************************************************/

#ifndef __GOBU_GOBU_EMBED_H__
#define __GOBU_GOBU_EMBED_H__
#include <gtk/gtk.h>
#include "bugo/bugo.h"

G_BEGIN_DECLS

typedef struct _GobuGobuEmbedPrivate GobuGobuEmbedPrivate;

#define GOBU_TYPE_GOBU_EMBED (gobu_gobu_embed_get_type())
G_DECLARE_FINAL_TYPE(GobuGobuEmbed, gobu_gobu_embed, GOBU, EMBED, GtkGLArea)

struct _GobuGobuEmbedPrivate
{
    GtkWidget parent_instance;
    guint width;
    guint height;
    gint tick;
    gboolean initialize;
    Color background_color;
};

GobuGobuEmbed *gobu_gobu_embed_new(void);
void gobu_gobu_embed_set_background_color(GobuGobuEmbed *embed, Color bg);

G_END_DECLS

#endif // __GOBU_GOBU_EMBED_H__
